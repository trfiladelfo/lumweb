//*****************************************************************************
//
// UARTIsr.c - Driver for UART block.
//
//*****************************************************************************

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"
#include "sysctl.h"
#include "ringbuf.h"

#include "UARTIsr.h"

//*****************************************************************************
//
//! The buffer used to hold characters received from the UART0.
//
//*****************************************************************************
static unsigned char RX0Buffer[RX_RING_BUF_SIZE];

//*****************************************************************************
//
//! The buffer used to hold characters to be sent to the UART0.
//
//*****************************************************************************
static unsigned char TX0Buffer[TX_RING_BUF_SIZE];

//*****************************************************************************
//
//! The buffer used to hold characters received from the UART1.
//
//*****************************************************************************
static unsigned char RX1Buffer[RX_RING_BUF_SIZE];

//*****************************************************************************
//
//! The buffer used to hold characters to be sent to the UART1.
//
//*****************************************************************************
static unsigned char TX1Buffer[TX_RING_BUF_SIZE];

//*****************************************************************************
//
//! The ring buffers used to hold characters received from the UARTs.
//
//*****************************************************************************
static tRingBufObject sRxBuf[MAX_UART_PORTS];

//*****************************************************************************
//
//! The ring buffers used to hold characters to be sent to the UARTs.
//
//*****************************************************************************
static tRingBufObject sTxBuf[MAX_UART_PORTS];

//*****************************************************************************
//
//! The base address for the UART associated with a port.
//
//*****************************************************************************
static const unsigned long UARTBase[MAX_UART_PORTS] =
{ UART0_BASE, UART1_BASE };

//*****************************************************************************
//
//! The interruopt for the UART associated with a port.
//
//*****************************************************************************
static const unsigned long UARTInterrupt[MAX_UART_PORTS] =
{ INT_UART0, INT_UART1 };

//*****************************************************************************
//
// This structure represents status of UART device. 
// Every state is defined in one bit. Access to these bits is via special function of 
// Cortex M3, bit band mapping.
//
//*****************************************************************************
static volatile unsigned long UARTDevice[MAX_UART_PORTS] =
{ 0, 0 };

//*****************************************************************************
//
// Informs service task about RX event from interrupt routine
//
//*****************************************************************************
static xSemaphoreHandle UARTRxBinSemaphore [MAX_UART_PORTS] =
{ NULL, NULL };

//*****************************************************************************
//
// Informs service task about TX event from interrupt routine
//
//*****************************************************************************
static xSemaphoreHandle UARTTxBinSemaphore [MAX_UART_PORTS] =
{ NULL, NULL };

//*****************************************************************************
//
// Prevents simultaneously accessing devices from different tasks 
//
//*****************************************************************************
static xSemaphoreHandle UARTAccessMutex[MAX_UART_PORTS] =
{ NULL, NULL };

//*****************************************************************************
//
//! Handles the UART interrupt.
//!
//! \param ulPort is the serial port number to be accessed.
//!
//! This function is called when either of the UARTs generate an interrupt.
//! An interrupt will be generated when data is received and when the transmit
//! FIFO becomes half empty.  The transmit and receive FIFOs are processed as
//! appropriate.
//!
//! \return None.
//
//*****************************************************************************
static unsigned long SerialUARTIntHandler(unsigned long ulPort)
{
	unsigned long ulStatus;
	unsigned long UARTEvent = NOP_EVENT;
	unsigned char ucChar;

	// Get the cause of the interrupt.
	ulStatus = UARTIntStatus(UARTBase[ulPort], true);

	// Clear the cause of the interrupt.
	UARTIntClear(UARTBase[ulPort], ulStatus);

	/*
	 See if there is data to be processed in the receive FIFO.    
	 The receive timeout interrupt is asserted when the receive FIFO is not empty, and no further data
	 is received over a 32-bit period. The receive timeout interrupt is cleared either when the FIFO
	 becomes empty through reading all the data (or by reading the holding register), or when a 1 is
	 written to the corresponding bit in the UARTICR register.
	 */
	if (ulStatus & (UART_INT_RT | UART_INT_RX))
	{
		// Loop while there are characters available in the receive FIFO.
		while (UARTCharsAvail(UARTBase[ulPort]))
		{
			// Get the next character from the receive FIFO.
			ucChar = UARTCharGetNonBlocking(UARTBase[ulPort]);

			// See if  there is space for characters in the receive FIFO.
			if (!(RingBufFull(&sRxBuf[ulPort])))
			{
				// Write byte to ring buffer
				RingBufWriteOne(&sRxBuf[ulPort], ucChar);
				//There was no error during UART operation
				HWREGBITW(&UARTDevice[ulPort], UART_ERROR) = 0;
			}
			else
			{
				//Inform service task that an error occured and no data was written
				HWREGBITW(&UARTDevice[ulPort], UART_ERROR) = 1;
			}
		}
		UARTEvent |= RX_EVENT;
	}
	
	// See if TX event occured.
	if (ulStatus & UART_INT_TX)
	{
		//There was no error during UART operation
		HWREGBITW(&UARTDevice[ulPort], UART_ERROR) = 0;

		UARTEvent |= TX_EVENT;
	}

	return UARTEvent;
}

//*****************************************************************************
//
//! Handles the UART0 interrupt.
//!
//! This function is called when the UART generates an interrupt.  
//!
//! \return None.
//
//*****************************************************************************
void SerialUART0IntHandler(void)
{
	// contains event status from serial port handler
	unsigned long UARTEvent;

	/*
	 * xSemaphoreGiveFromISR() will set *pxHigherPriorityTaskWoken to pdTRUE 
	 * if giving the semaphoree caused a task to unblock, and the unblocked 
	 * task has a priority higher than the currently running task.
	 */
	static portBASE_TYPE xHigherPriorityTaskWoken;

	// call serial port handler for UART 0
	UARTEvent = SerialUARTIntHandler(0);

	if (RX_EVENT & UARTEvent)
	{
		// receive event occured, Rx Int status or no more data received and timeout expired
		// wake up read function
		xSemaphoreGiveFromISR( UARTRxBinSemaphore[0], &xHigherPriorityTaskWoken );
	}

	if (TX_EVENT & UARTEvent)
	{
		// transmitt event occured, Tx Int status
		// wake up write function
		xSemaphoreGiveFromISR( UARTTxBinSemaphore[0], &xHigherPriorityTaskWoken );
	}

	/* If a task was woken by the character being received then we force
	 a context switch to occur in case the task is of higher priority than
	 the currently executing task (i.e. the task that this interrupt
	 interrupted.) 
	 */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

//*****************************************************************************
//
//! Handles the UART1 interrupt.
//!
//! This function is called when the UART generates an interrupt.  
//!
//! \return None.
//
//*****************************************************************************
void SerialUART1IntHandler(void)
{
	// contains event status from serial port handler
	unsigned long UARTEvent;

	/*
	 * xSemaphoreGiveFromISR() will set *pxHigherPriorityTaskWoken to pdTRUE 
	 * if giving the semaphoree caused a task to unblock, and the unblocked 
	 * task has a priority higher than the currently running task.
	 */
	static portBASE_TYPE xHigherPriorityTaskWoken;

	// call serial port handler for UART 1
	UARTEvent = SerialUARTIntHandler(1);

	if (RX_EVENT & UARTEvent)
	{
		// receive event occured, Rx Int status or no more data received and timeout expired
		// wake up read function
		xSemaphoreGiveFromISR( UARTRxBinSemaphore[1], &xHigherPriorityTaskWoken );
	}

	if (TX_EVENT & UARTEvent)
	{
		// transmitt event occured, Tx Int status
		// wake up write function
		xSemaphoreGiveFromISR( UARTTxBinSemaphore[1], &xHigherPriorityTaskWoken );
	}

	/* If a task was woken by the character being received then we force
	 a context switch to occur in case the task is of higher priority than
	 the currently executing task (i.e. the task that this interrupt
	 interrupted.) 
	 */
	portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

//*****************************************************************************
//
//! Enables transmitting and receiving.
//!
//! \param ulPort is the UART port number to be accessed.
//!
//! Sets the UARTEN, and RXE bits, and enables the transmit and receive
//! FIFOs.
//!
//! \return None.
//
//*****************************************************************************
static void UARTServiceTaskEnable(unsigned long ulPort)
{

	// Cofigures UART to default values, 19200,N,8
	UARTConfigSetExpClk(UARTBase[ulPort], SysCtlClockGet(), 19200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	// Sets FIFO levels interrupt.
	UARTFIFOLevelSet(UARTBase[ulPort], UART_FIFO_TX1_8, UART_FIFO_RX7_8);

	// (Re)enable the uart transmit and receive interrupts.
	IntEnable(UARTInterrupt[ulPort]);

	//Clear int source to avoid unwanted interrupts
	UARTIntClear(UARTBase[ulPort], (UART_INT_RX | UART_INT_RT | UART_INT_TX | UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE));

	// (Re)enable the uart transmit and receive interrupts.
	UARTIntEnable(UARTBase[ulPort], (UART_INT_RX | UART_INT_RT));

}

//*****************************************************************************
//
//! Disables transmitting and receiving.
//!
//! \param ulPort is the UART port number to be accessed.
//!
//! Sets the UARTEN, and RXE bits, and disables the transmit and receive
//! FIFOs.
//!
//! \return None.
//
//*****************************************************************************
static void UARTServiceTaskDisable(unsigned long ulPort)
{
	// Disable the uart transmit and receive interrupts.
	IntDisable(UARTInterrupt[ulPort]);

	// Disable the uart transmit and receive interrupts.
	UARTIntDisable(UARTBase[ulPort], (UART_INT_RX | UART_INT_RT | UART_INT_TX));

	// Disable the UART.
	UARTDisable(UARTBase[ulPort]);

	// Purge the receive data.
	RingBufFlush(&sRxBuf[ulPort]);

	// Purge the transmit data.
	RingBufFlush(&sTxBuf[ulPort]);
}

//*****************************************************************************
//
//! Initializes the serial port driver.
//!
//! This function initializes and configures the serial port driver.
//!
//! \return None.
//
//*****************************************************************************
void UART0ServiceTaskInit(void)
{
	// Initialize the ring buffers used by the UART Drivers.
	RingBufInit(&sRxBuf[0], RX0Buffer, sizeof(RX0Buffer));
	RingBufInit(&sTxBuf[0], TX0Buffer, sizeof(TX0Buffer));

	// Initialize semaphores and mutexes.
	UARTRxBinSemaphore[0] = xSemaphoreCreateCounting( 1, 0 );
	UARTTxBinSemaphore[0] = xSemaphoreCreateCounting( 1, 0 );
	UARTAccessMutex[0] = xSemaphoreCreateMutex();

	// Enable peripheral, other fault is generated
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	// Configure the Port A, pins 0,2 appropriately.
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Set interrupt priority to number higher than configMAX_SYSCALL_INTERRUPT_PRIORITY
	// defined in FreeRTOSConfig.h, see www.freertos.org
	IntPrioritySet(UARTInterrupt[0], SET_SYSCALL_INTERRUPT_PRIORITY(6));

}

//*****************************************************************************
//
//! Initializes the serial port driver.
//!
//! This function initializes and configures the serial port driver.
//!
//! \return None.
//
//*****************************************************************************
void UART1ServiceTaskInit(void)
{

	// Initialize the ring buffers used by the UART Drivers.
	RingBufInit(&sRxBuf[1], RX1Buffer, sizeof(RX1Buffer));
	RingBufInit(&sTxBuf[1], TX1Buffer, sizeof(TX1Buffer));

	// Initialize semaphores and mutexes.
	UARTRxBinSemaphore[1] = xSemaphoreCreateCounting( 1, 0 );
	UARTTxBinSemaphore[1] = xSemaphoreCreateCounting( 1, 0 );
	UARTAccessMutex[1] = xSemaphoreCreateMutex();

	// Enable peripheral, other fault is generated
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	// Configure the Port D, pins 2,3 appropriately.
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);

	// Set interrupt priority to number higher than configMAX_SYSCALL_INTERRUPT_PRIORITY
	// defined in FreeRTOSConfig.h, see www.freertos.org
	IntPrioritySet(UARTInterrupt[1], SET_SYSCALL_INTERRUPT_PRIORITY(6));

}

//*****************************************************************************
//
//! Opens UART device.
//!
//! \param None.
//!
//! This function opens device for operating.
//!
//! \return Opened device identifier or -1 if error.
//
//*****************************************************************************
int UARTServiceTaskOpen(unsigned long ulPort)
{
	int retVal = -1;

	if (ulPort < MAX_UART_PORTS)
	{
		// Access to shared variable   	
		xSemaphoreTake( UARTAccessMutex[ulPort], ( portTickType ) portMAX_DELAY);

		if ((0 == HWREGBITW(&UARTDevice[ulPort], UART_OPENED)))
		{
			// Purge the receive data.
			RingBufFlush(&sRxBuf[ulPort]);

			// Purge the transmit data.
			RingBufFlush(&sTxBuf[ulPort]);

			// Set device as opened
			HWREGBITW(&UARTDevice[ulPort], UART_OPENED) = 1;

			// Enable UART
			UARTServiceTaskEnable(ulPort);

			// return ok, number of opened device
			retVal = ulPort;
		}
		// We have finished accessing the shared resource.  Release the semaphore.
		xSemaphoreGive( UARTAccessMutex[ulPort]);
	}
	// returns default if operation was not successful  
	return retVal;
}

//*****************************************************************************
//
//! Receives a character from the UART.
//!
//! \param ulPort is the UART port number to be accessed.
//! \param UARTdata is pointer to data where will be written to.
//! \param UARTDataCount how many bytes should be read.
//!
//! This function reads a character(s) from the receive buffer.
//!
//! \return Number of read bytes or -1 if error.
//
//*****************************************************************************
int UARTServiceTaskRead(unsigned long ulPort, unsigned char * UARTdata, unsigned long UARTDataCount)
{
	unsigned long UARTByteReceived = 0;

	if ((ulPort < MAX_UART_PORTS) && (1
			== HWREGBITW(&UARTDevice[ulPort], UART_OPENED)))
	{
		while (UARTDataCount)
		{
			if (RingBufEmpty(&sRxBuf[ulPort]))
			{
				// No data received yet
				GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, GPIO_PIN_4);

				// Waiting for finishing receiving from interrupt routine 
				xSemaphoreTake(UARTRxBinSemaphore[ulPort], ( portTickType ) portMAX_DELAY);

				GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_4, 0);
			}
			else
			{
				// removes byte from buffer and stores 
				*UARTdata = RingBufReadOne(&sRxBuf[ulPort]);

				//Move pointer to the next data
				UARTdata++;

				// Decrease value how many bytes should be read
				UARTDataCount--;

				// Increase vaule how many bytes have been read
				UARTByteReceived++;
			}
		}

		// returns how many bytes has been read
		return UARTByteReceived;
	}
	return -1;
}

//*****************************************************************************
//
//! Sends a character to the UART.
//!
//! \param ulPort is the UART port number to be accessed.
//! \param UARTdata is pointer to data that will be read.
//! \param UARTDataCount how many bytes should be read.
//!
//! This function sends a character to the UART.  The character will either be
//! directly written into the UART FIFO or into the UART transmit buffer, as
//! appropriate.
//!
//! \return Number of written bytes or -1 if error.
//
//*****************************************************************************
int UARTServiceTaskWrite(unsigned long ulPort, unsigned char * UARTdata, unsigned long UARTDataCount)
{
	unsigned long UARTByteWritten = 0;

	if ((ulPort < MAX_UART_PORTS) && (1
			== HWREGBITW(&UARTDevice[ulPort], UART_OPENED)))
	{
		while (UARTDataCount)
		{
			// See if there is space in the FIFO.		
			if ((UARTSpaceAvail(UARTBase[ulPort])))
			{
				//
				// Write this character directly into the FIFO.
				//
				UARTCharPutNonBlocking(UARTBase[ulPort], *UARTdata);

				//Move pointer to the next data
				UARTdata++;

				// Decrease value how many bytes should be written
				UARTDataCount--;

				// Increase vaule how many bytes have been written
				UARTByteWritten++;

			}
			else
			{
				// Enable interrupt to generate tx event
				UARTIntEnable(UARTBase[ulPort], UART_INT_TX);

				// Waiting for finishing transmitting from interrupt routine
				xSemaphoreTake(UARTTxBinSemaphore[ulPort], ( portTickType ) portMAX_DELAY);

				// Disable the UART transmit interrupt while determining how to handle this
				// character.  Failure to do so could result in the loss of this character,
				// or stalled output due to this character being placed into the UART
				// transmit buffer but never transferred out into the UART FIFO.
				UARTIntDisable(UARTBase[ulPort], UART_INT_TX);
			}
		} //while (UARTDataCount)

		// returns how many bytes has been read
		return UARTByteWritten;

	} // if ((ulPort < MAX_S2E_PORTS) && (0 == HWREGBITW(&UARTDevice[ulPort], UART_OPENED)))

	return -1;
}

//*****************************************************************************
//
//! Configures UART device.
//!
//! \param None.
//!
//! This function configures opened device.
//!
//! \return 0 or -1 if error.
//
//*****************************************************************************
int UARTServiceTaskConfig(unsigned long ulPort, UART_CONFIG * config)
{
	if ((ulPort < MAX_UART_PORTS) && (1
			== HWREGBITW(&UARTDevice[ulPort], UART_OPENED)))
	{
		// Disable the UART.
		UARTDisable(UARTBase[ulPort]);

		// Cofigures UART, UARTConfigSetExpClk enables UART
		UARTConfigSetExpClk(UARTBase[ulPort], SysCtlClockGet(), config->baudRate, (config->wordLen
				| config->stop | config->parity));

		//ok
		return 0;
	}
	// return error
	return -1;
}

//*****************************************************************************
//
//! Closes UART device.
//!
//! \param None.
//!
//! This function closes devic.
//!
//! \return 0 or -1 if error.
//
//*****************************************************************************
int UARTServiceTaskClose(unsigned long ulPort)
{
	int retVal = -1;

	if (ulPort < MAX_UART_PORTS)
	{
		// Access to shared variable   	
		xSemaphoreTake( UARTAccessMutex[ulPort], ( portTickType ) portMAX_DELAY);

		if (1 == HWREGBITW(&UARTDevice[ulPort], UART_OPENED))
		{
			// Disable UART
			UARTServiceTaskDisable(ulPort);

			// Reset device status
			UARTDevice[ulPort] = 0;

			// return ok
			retVal = 0;
		}
		// We have finished accessing the shared resource.  Release the semaphore.
		xSemaphoreGive(UARTAccessMutex[ulPort]);
	}
	// returns default if operation was not successful  
	return retVal;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
