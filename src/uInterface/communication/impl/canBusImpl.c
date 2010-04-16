/**
 * \addtogroup comTask
 * @{
 *
 * \author Anziner, Hahn
 * \brief implements a simple testroutine to store and read the values from/to the SD-Card
 *
 *
 */

/* std lib includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "setup.h"

#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_sysctl.h"
#include "sysctl.h"

#include "gpio.h"

#include "can.h"
#include "hw_can.h"

#include "interrupt.h"
#include "hw_ints.h"

#include "../comTask.h"

int CANTransmitFIFO(unsigned char *pucData, unsigned long ulSize);
int CANReceiveFIFO(unsigned char *pucData, unsigned long ulSize);

//
// Size of the FIFOs allocated to the CAN controller.
//
#define CAN_FIFO_SIZE           (8 * 8)

//
// Message object used by the transmit message FIFO.
//
#define TRANSMIT_MESSAGE_ID     8

//
// Message object used by the receive message FIFO.
//
#define RECEIVE_MESSAGE_ID      11

//
// The CAN bit rate.
//
#define CAN_BITRATE             250000

#define MSG_OBJ_FIFO			0 // TODO only testing
//
// This structure holds all of the state information for the CAN transfers.
//
struct
{
	//
	// This holds the information for the data receive message object that is
	// used to receive data for each CAN controller.
	//
	tCANMsgObject MsgObjectRx;

	//
	// This holds the information for the data send message object that is used
	// to send data for each CAN controller.
	//
	tCANMsgObject MsgObjectTx;

	//
	// Receive buffer.
	//
	unsigned char pucBufferRx[CAN_FIFO_SIZE];

	//
	// Transmit buffer.
	//
	unsigned char pucBufferTx[CAN_FIFO_SIZE];

	//
	// Bytes remaining to be received.
	//
	unsigned long ulBytesRemaining;

	//
	// Bytes transmitted.
	//
	unsigned long ulBytesTransmitted;

	//
	// The current state of the CAN controller.
	//
	enum
	{
		CAN_IDLE, CAN_SENDING, CAN_WAIT_RX, CAN_PROCESS,
	} eState;
} g_sCAN;

//
// Used by the ToggleLED function to set the toggle rate.
//
unsigned long g_ulLEDCount;

void vComTaskInitImpl(void)
{
	//
	// Configure CAN 0 Pins.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	//
	// Enable the CAN controller.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

	//
	// Reset the state of all the message object and the state of the CAN
	// module to a known state.
	//
	CANInit(CAN0_BASE);

	//
	// Configure the bit rate for the CAN device, the clock rate to the CAN
	// controller is fixed at 8MHz for this class of device and the bit rate is
	// set to CAN_BITRATE.
	//
	CANBitRateSet(CAN0_BASE, 8000000, CAN_BITRATE);

	//
	// Take the CAN0 device out of INIT state.
	//
	CANEnable(CAN0_BASE);

	//
	// Enable interrupts from CAN controller.
	//
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR);

	//
	// Enable interrupts for the CAN in the NVIC.
	//
	IntEnable(INT_CAN0);

	//
	// Enable processor interrupts.
	//
	IntMasterEnable();
}

int sendToMachine(char* id, int value)
{

	int rc = 0;
	char buffer[32];

	// suspend all other tasks
	vTaskSuspendAll();

	snprintf(buffer, 32, "%s:%d", id, value);
	CANTransmitFIFO((unsigned char*) buffer, (unsigned long) strlen(buffer));
	// resumes all tasks
	xTaskResumeAll();

	return rc;
}

int getFormMachine(char* id)
{
	int value = -999; // error code

	// suspend all other tasks
	vTaskSuspendAll();

	// resumes all tasks
	xTaskResumeAll();

	return value;
}

//*****************************************************************************
//
// The CAN controller interrupt handler.
//
//*****************************************************************************
void CANIntHandler(void)
{
	unsigned long ulStatus;

	//
	// Find the cause of the interrupt, if it is a status interrupt then just
	// acknowledge the interrupt by reading the status register.
	//
	ulStatus = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

	//
	// The first eight message objects make up the Transmit message FIFO.
	//
	if (ulStatus <= 8)
	{
		//
		// Increment the number of bytes transmitted.
		//
		g_sCAN.ulBytesTransmitted += 8;
	}
	//
	// The second eight message objects make up the Receive message FIFO.
	//
	else if ((ulStatus > 8) && (ulStatus <= 16))
	{
		//
		// Read the data out and acknowledge that it was read.
		//
		CANMessageGet(CAN0_BASE, ulStatus, &g_sCAN.MsgObjectRx, 1);

		//
		// Advance the read pointer.
		//
		g_sCAN.MsgObjectRx.pucMsgData += 8;

		// todo Test
		printf("CAN MESSAGE: %s", g_sCAN.MsgObjectRx.pucMsgData);

		//
		// Decrement the expected bytes remaining.
		//
		g_sCAN.ulBytesRemaining -= 8;
	}
	else
	{
		//
		// This was a status interrupt so read the current status to
		// clear the interrupt and return.
		//
		CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
	}

	//
	// Acknowledge the CAN controller interrupt has been handled.
	//
	CANIntClear(CAN0_BASE, ulStatus);
}

//*****************************************************************************
//
// This function configures the transmit FIFO and copies data into the FIFO.
//
//*****************************************************************************
int CANTransmitFIFO(unsigned char *pucData, unsigned long ulSize)
{
	int iIdx;

	//
	// This is the message object used to send button updates.  This message
	// object will not be "set" right now as that would trigger a transmission.
	//
	g_sCAN.MsgObjectTx.ulMsgID = TRANSMIT_MESSAGE_ID;
	g_sCAN.MsgObjectTx.ulMsgIDMask = 0;

	//
	// This enables interrupts for transmitted messages.
	//
	g_sCAN.MsgObjectTx.ulFlags = MSG_OBJ_TX_INT_ENABLE;

	//
	// Return the maximum possible number of bytes that can be sent in a single
	// FIFO.
	//
	if (ulSize > CAN_FIFO_SIZE)
	{
		return (CAN_FIFO_SIZE);
	}

	//
	// Loop through all eight message objects that are part of the transmit
	// FIFO.
	//
	for (iIdx = 0; iIdx < 8; iIdx++)
	{
		//
		// If there are more than eight bytes remaining then use a full message
		// to transfer these 8 bytes.
		//
		if (ulSize > 8)
		{
			//
			// Set the length of the message, which can only be eight bytes
			// in this case as it is all that can be sent with a single message
			// object.
			//
			g_sCAN.MsgObjectTx.ulMsgLen = 8;
			g_sCAN.MsgObjectTx.pucMsgData = &pucData[iIdx * 8];

			//
			// Set the MSG_OBJ_FIFO to indicate that this is not the last
			// data in a chain of FIFO entries.
			//
			g_sCAN.MsgObjectTx.ulFlags |= MSG_OBJ_FIFO;

			//
			// There are now eight less bytes to transmit.
			//
			ulSize -= 8;

			//
			// Write out this message object.
			//
			CANMessageSet(CAN0_BASE, iIdx + 1, &g_sCAN.MsgObjectTx,
					MSG_OBJ_TYPE_TX);
		}
		//
		// If there are less than or exactly eight bytes remaining then use a
		// message object to transfer these 8 bytes and do not set the
		// MSG_OBJ_FIFO flag to indicate that this is the last of the entries
		// in this FIFO.
		//
		else
		{
			//
			// Set the length to the remaining bytes and transmit the data.
			//
			g_sCAN.MsgObjectTx.ulMsgLen = ulSize;
			g_sCAN.MsgObjectTx.pucMsgData = &pucData[iIdx * 8];

			//
			// Write out this message object.
			//
			CANMessageSet(CAN0_BASE, iIdx + 1, &g_sCAN.MsgObjectTx,
					MSG_OBJ_TYPE_TX);
		}
	}
	return (0);
}

//*****************************************************************************
//
// This function configures the receive FIFO and should only be called once.
//
//*****************************************************************************
int CANReceiveFIFO(unsigned char *pucData, unsigned long ulSize)
{
	int iIdx;

	if (ulSize > CAN_FIFO_SIZE)
	{
		return (CAN_FIFO_SIZE);
	}

	//
	// Configure the receive message FIFO to accept the transmit message object.
	//
	g_sCAN.MsgObjectRx.ulMsgID = RECEIVE_MESSAGE_ID;
	g_sCAN.MsgObjectRx.ulMsgIDMask = 0;

	//
	// This enables interrupts for received messages.
	//
	g_sCAN.MsgObjectRx.ulFlags = MSG_OBJ_RX_INT_ENABLE;

	//
	// Remember the beginning of the FIFO location.
	//
	g_sCAN.MsgObjectRx.pucMsgData = pucData;

	//
	// Transfer bytes in multiples of eight bytes.
	//
	for (iIdx = 0; iIdx < (CAN_FIFO_SIZE / 8); iIdx++)
	{
		//
		// If there are more than eight remaining to be sent then just queue up
		// eight bytes and go on to the next message object(s) for the
		// remaining bytes.
		//
		if (ulSize > 8)
		{
			//
			// The length is always eight as the full buffer is divisible by 8.
			//
			g_sCAN.MsgObjectRx.ulMsgLen = 8;

			//
			// There are now eight less bytes to receive.
			//
			ulSize -= 8;

			//
			// Set the MSG_OBJ_FIFO to indicate that this is not the last
			// data in a chain of FIFO entries.
			//
			g_sCAN.MsgObjectRx.ulFlags |= MSG_OBJ_FIFO;

			//
			// Make sure that all message objects up to the last indicate that
			// they are part of a FIFO.
			//
			CANMessageSet(CAN0_BASE, iIdx + 9, &g_sCAN.MsgObjectRx,
					MSG_OBJ_TYPE_RX);
		}
		else
		{
			//
			// Get the remaining bytes.
			//
			g_sCAN.MsgObjectRx.ulMsgLen = ulSize;

			//
			// This is the last message object in a FIFO so don't set the FIFO
			// to indicate that the FIFO ends with this message object.
			//
			CANMessageSet(CAN0_BASE, iIdx + 9, &g_sCAN.MsgObjectRx,
					MSG_OBJ_TYPE_RX);
		}
	}
	return (0);
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

