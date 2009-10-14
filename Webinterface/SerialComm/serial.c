/* Scheduler include files. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* Demo app include files. */
#include "serial.h"

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_uart.h"
#include "sysctl.h"
#include "interrupt.h"
#include "uart.h"

#include "debug.h"

#include <stdarg.h>

//The queues used to communicate between tasks and ISR's.
static xQueueHandle xRxQueue;
static xQueueHandle xTxQueue;

//Used to store the peripheral addresses
unsigned long UART_SYSCTL_PERIPH;
unsigned long UART_BASE;
unsigned long UART_INT;

void SerialPortInit(int port, u32 Baudrate, u16 RxQueueLen, u16 TxQueueLen)
{

	switch (port)
	{
	case 0:
		UART_SYSCTL_PERIPH = SYSCTL_PERIPH_UART0;
		UART_BASE = UART0_BASE;
		UART_INT = INT_UART0;
		break;
	case 1:
		UART_SYSCTL_PERIPH = SYSCTL_PERIPH_UART1;
		UART_BASE = UART1_BASE;
		UART_INT = INT_UART1;
		break;
	case 2:
		UART_SYSCTL_PERIPH = SYSCTL_PERIPH_UART2;
		UART_BASE = UART2_BASE;
		UART_INT = INT_UART2;
		break;
	default:
		ASSERT(false);
		break;
	}

	//Create the queues used by the serial driver
	xRxQueue = xQueueCreate(RxQueueLen, (unsigned portBASE_TYPE ) sizeof(u8));
	xTxQueue = xQueueCreate(TxQueueLen, (unsigned portBASE_TYPE ) sizeof(u8));

	SysCtlPeripheralEnable(UART_SYSCTL_PERIPH);
	UARTConfigSet(UART_BASE, Baudrate, UART_CONFIG_PAR_NONE |
			UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8);

	UARTFIFOLevelSet(UART_BASE, UART_FIFO_TX1_8, UART_FIFO_RX4_8);

	//Clear the Rx buffer
	while (UARTCharsAvail(UART_BASE))
	{
		UARTCharNonBlockingGet(UART_BASE);
	}

	//Enable the interrupt
	IntEnable(UART_INT);
	IntPrioritySet(UART_INT, configKERNEL_INTERRUPT_PRIORITY);
	UARTIntEnable(UART_BASE, UART_INT_RX | UART_INT_TX | UART_INT_RT);

}

BOOL SerialGetChar(u8* pChar, portTickType timeout)
{
	if (xQueueReceive( xRxQueue, pChar, timeout ))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

u16 SerialGetArray(u8* pBuf, u16 len, portTickType timeout)
{
	u16 i;

	for (i = 0; i < len; i++)
	{
		if (!xQueueReceive( xRxQueue, &pBuf[i], timeout ))
		{
			return i;
		}
	}
	return i;
}

u16 SerialGetLine(u8* pBuf, u16 len, portTickType timeout)
{
	u16 i;

	for (i = 0; i < len;)
	{
		if (!xQueueReceive( xRxQueue, &pBuf[i], timeout ))
		{
			return 0;
		}
		if (pBuf[i] == '\n')
		{
			pBuf[i] = 0;
			return i;
		}
		else if (pBuf[i] == '\r')
		{
			//Do nothing
		}
		else
		{
			i++;
			if (i >= len)
			{
				return 0;
			}
		}
	}
	return 0;
}

u16 SerialGetLineWithEcho(u8* pBuf, u16 len, portTickType timeout)
{
	u16 i;

	for (i = 0; i < len;)
	{
		if (!xQueueReceive( xRxQueue, &pBuf[i], timeout ))
		{
			return 0;
		}
		if (pBuf[i] == '\n')
		{
			pBuf[i] = 0;
			SerialPutChar(pBuf[i], timeout);
			return i;
		}
		else if (pBuf[i] == '\r')
		{
			SerialPutChar(pBuf[i], timeout);
		}
		else
		{
			SerialPutChar(pBuf[i], timeout);
			i++;
			if (i >= len)
			{
				return 0;
			}
		}
	}
	return 0;
}

u16 SerialRxCharCount(void)
{
	return (u16) uxQueueMessagesWaiting(xRxQueue);
}

BOOL SerialPutChar(u8 Char, portTickType timeout)
{

	//Return false if after the block time there is no room on the Tx queue.
	if (xQueueSend( xTxQueue, &Char, timeout) != pdPASS)
	{
		return FALSE;
	}

	//Disable the interrupt
	UARTIntDisable(UART_BASE, UART_INT_TX);

	while (UARTSpaceAvail(UART_BASE))
	{
		if (xQueueReceive( xTxQueue, &Char, 0 ) == pdTRUE)
		{
			//Send the next character queued for Tx
			UARTCharNonBlockingPut(UART_BASE, Char);
		}
		else
		{
			break;
		}
	}

	//Enable the interrupt back
	UARTIntEnable(UART_BASE, UART_INT_TX);

	return TRUE;
}

u16 SerialPutArray(u8* pBuf, u16 len, portTickType timeout)
{
	u16 i;
	u8 Char;

	for (i = 0; i < len;)
	{
		//Fill the queue
		while (xQueueSend( xTxQueue, &pBuf[i], 0) == pdPASS)
		{
			i++;
			if (i >= len)
			{
				break;
			}
		}

		//Disable the interrupt and send one char to
		//ensure the Tx interrupt mechanism is started
		UARTIntDisable(UART_BASE, UART_INT_TX);
		while (UARTSpaceAvail(UART_BASE))
		{
			if (xQueueReceive( xTxQueue, &Char, 0 ) == pdTRUE)
			{
				//Send the next character queued for Tx
				UARTCharNonBlockingPut(UART_BASE, Char);
			}
			else
			{
				break;
			}
		}
		//Enable the interrupt back
		UARTIntEnable(UART_BASE, UART_INT_TX);

		if (i < len)
		{
			if (xQueueSend( xTxQueue, &pBuf[i], timeout) != pdPASS)
			{
				return i;
			}
			i++;
		}
	}

	return i;
}

void SerialPrintf(char* pString, ...)
{
	unsigned long ulIdx, ulValue, ulPos, ulCount, ulBase;
	char *pcStr, pcBuf[16], cFill;
	va_list vaArgP;
	static const char * const g_pcHex = "0123456789abcdef";

	//
	// Start the varargs processing.
	//
	va_start(vaArgP, pString);

	//
	// Loop while there are more characters in the string.
	//
	while (*pString)
	{
		//
		// Find the first non-% character, or the end of the string.
		//
		for (ulIdx = 0; (pString[ulIdx] != '%') && (pString[ulIdx] != '\0'); ulIdx++)
		{
		}

		//
		// Write this portion of the string.
		//
		SerialPutArray((u8*) pString, ulIdx, portMAX_DELAY);

		//
		// Skip the portion of the string that was written.
		//
		pString += ulIdx;

		//
		// See if the next character is a %.
		//
		if (*pString == '%')
		{
			//
			// Skip the %.
			//
			pString++;

			//
			// Set the digit count to zero, and the fill character to space
			// (i.e. to the defaults).
			//
			ulCount = 0;
			cFill = ' ';

			//
			// It may be necessary to get back here to process more characters.
			// Goto's aren't pretty, but effective.  I feel extremely dirty for
			// using not one but two of the beasts.
			//
			again:

			//
			// Determine how to handle the next character.
			//
			switch (*pString++)
			{
			//
			// Handle the digit characters.
			//
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			{
				//
				// If this is a zero, and it is the first digit, then the
				// fill character is a zero instead of a space.
				//
				if ((pString[-1] == '0') && (ulCount == 0))
				{
					cFill = '0';
				}

				//
				// Update the digit count.
				//
				ulCount *= 10;
				ulCount += pString[-1] - '0';

				//
				// Get the next character.
				//
				goto again;
			}

				//
				// Handle the %c command.
				//
			case 'c':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Print out the character.
				//
				SerialPutArray((u8*) &ulValue, 1, portMAX_DELAY);

				//
				// This command has been handled.
				//
				break;
			}

				//
				// Handle the %d command.
				//
			case 'd':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Reset the buffer position.
				//
				ulPos = 0;

				//
				// If the value is negative, make it positive and stick a
				// minus sign in the beginning of the buffer.
				//
				if ((long) ulValue < 0)
				{
					pcBuf[ulPos++] = '-';
					ulValue = -(long) ulValue;
				}

				//
				// Set the base to 10.
				//
				ulBase = 10;

				//
				// Convert the value to ASCII.
				//
				goto convert;
			}

				//
				// Handle the %s command.
				//
			case 's':
			{
				//
				// Get the string pointer from the varargs.
				//
				pcStr = va_arg(vaArgP, char *);

				//
				// Determine the length of the string.
				//
				for (ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++, ulCount--)
				{
				}

				//
				// Provide additional padding at the beginning of the
				// string conversion if needed.
				//
				if ((ulCount > 1) && (ulCount < 16))
				{
					for (ulCount--; ulCount; ulCount--)
					{
						SerialPutChar(cFill, portMAX_DELAY);
					}
				}

				//
				// Write the string.
				//
				SerialPutArray((u8*) pcStr, ulIdx, portMAX_DELAY);

				//
				// This command has been handled.
				//
				break;
			}

				//
				// Handle the %u command.
				//
			case 'u':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Reset the buffer position.
				//
				ulPos = 0;

				//
				// Set the base to 10.
				//
				ulBase = 10;

				//
				// Convert the value to ASCII.
				//
				goto convert;
			}

				//
				// Handle the %x and %X commands.  Note that they are treated
				// identically; i.e. %X will use lower case letters for a-f
				// instead of the upper case letters is should use.
				//
			case 'x':
			case 'X':
			{
				//
				// Get the value from the varargs.
				//
				ulValue = va_arg(vaArgP, unsigned long);

				//
				// Reset the buffer position.
				//
				ulPos = 0;

				//
				// Set the base to 16.
				//
				ulBase = 16;

				//
				// Determine the number of digits in the string version of
				// the value.
				//
				convert: for (ulIdx = 1; (((ulIdx * ulBase) <= ulValue) && (((ulIdx * ulBase) / ulBase) == ulIdx)); ulIdx *= ulBase, ulCount--)
				{
				}

				//
				// Provide additional padding at the beginning of the
				// string conversion if needed.
				//
				if ((ulCount > 1) && (ulCount < 16))
				{
					for (ulCount--; ulCount; ulCount--)
					{
						pcBuf[ulPos++] = cFill;
					}
				}

				//
				// Convert the value into a string.
				//
				for (; ulIdx; ulIdx /= ulBase)
				{
					pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];
				}

				//
				// Write the string.
				//
				SerialPutArray((u8*) pcBuf, ulPos, portMAX_DELAY);

				//
				// This command has been handled.
				//
				break;
			}

				//
				// Handle the %% command.
				//
			case '%':
			{
				//
				// Simply write a single %.
				//
				SerialPutArray((u8*) pString - 1, 1, portMAX_DELAY);

				//
				// This command has been handled.
				//
				break;
			}

				//
				// Handle all other commands.
				//
			default:
			{
				//
				// Indicate an error.
				//
				SerialPutArray((u8*) "ERROR", 5, portMAX_DELAY);

				//
				// This command has been handled.
				//
				break;
			}
			}
		}
	}

	//
	// End the varargs processing.
	//
	va_end(vaArgP);
}

u16 SerialTxCharCount(void)
{
	return (u16) uxQueueMessagesWaiting(xTxQueue);
}

__attribute__((__interrupt__)) void UARTIntHandler(void)
{
	unsigned long ulTemp;
	portBASE_TYPE xTaskRxWoken = pdFALSE;
	portBASE_TYPE xTaskTxWoken = pdFALSE;
	u8 value;

	//
	// Read and Clear the interrupt.
	//
	ulTemp = UARTIntStatus(UART_BASE, false);
	UARTIntClear(UART_BASE, ulTemp);

	//
	// Check to see if an RX Interrupt has occured.
	//
	if (ulTemp & (UART_INT_RX | UART_INT_RT))
	{
		while (UARTCharsAvail(UART_BASE))
		{
			value = (u8) UARTCharNonBlockingGet(UART_BASE);
			xQueueSendFromISR( xRxQueue, &value, &xTaskRxWoken );
		}
	}

	//
	// Check to see if an TX Interrupt has occurred.
	//
	if (ulTemp & UART_INT_TX)
	{
		while (UARTSpaceAvail(UART_BASE))
		{
			if (xQueueReceiveFromISR(xTxQueue, &value, &xTaskTxWoken) == pdTRUE)
			{
				// Send the next character queued for Tx.
				UARTCharNonBlockingPut(UART_BASE, value);
				//UARTCharPut(UART_BASE, value);
			}
			else
			{
				break;
			}
		}
	}

	if ((xTaskRxWoken != pdFALSE) || (xTaskTxWoken != pdFALSE))
	{
		taskYIELD();
	}
}

