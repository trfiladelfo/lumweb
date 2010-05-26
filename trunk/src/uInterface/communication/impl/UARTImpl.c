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

#include "../comTask.h"
#include "setup.h"

#include "hw_types.h"
#include "hw_gpio.h"
#include "hw_can.h"
#include "hw_memmap.h"
#include "hw_ints.h"

#include "can.h"
#include "gpio.h"
#include "sysctl.h"
#include "interrupt.h"


tCANMsgObject g_MsgObjectRx;

#define BUFFER_LEN 32

char ucBufferIn[BUFFER_LEN];

int ulStatus = 0;


void CANIntHandler(void)
{
    CANIntClear(CAN0_BASE, ulStatus);
}


void vComTaskInitImpl(void)
{


}

int sendToMachine(char* id, int value)
{
	int rc = 0;
	
	UARTprintf("!s:%s=%d", id, value);

	
	return rc;
}


int getFormMachine(char* id)
{
	int value = -999; // error code
	char read_buf[32];
	
	UARTprintf("!g:%s\n", id);

	UARTgets(&read_buf, 32);

	UARTprintf("READ from Machine: '%s'\n", read_buf);

	value = atoi(read_buf);
	return value;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

