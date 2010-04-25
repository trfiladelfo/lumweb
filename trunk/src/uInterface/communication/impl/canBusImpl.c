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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
 
    GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    CANInit(CAN0_BASE);

    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 1000000);
	
    CANEnable(CAN0_BASE);

    CANIntEnable(CAN0_BASE, CAN_INT_MASTER);
   
	IntEnable(INT_CAN0);

    IntMasterEnable();

}

int sendToMachine(char* id, int value)
{
	int rc = 0;
	
	g_MsgObjectRx.ulMsgID = (0x400);
    g_MsgObjectRx.ulMsgIDMask = 0x7f8;        
    g_MsgObjectRx.ulFlags = MSG_OBJ_USE_ID_FILTER;
    g_MsgObjectRx.ulMsgLen = snprintf(ucBufferIn, BUFFER_LEN, "%s:%d", id, value);            
    g_MsgObjectRx.pucMsgData = ucBufferIn;
	
	CANMessageSet(CAN0_BASE, 1, &g_MsgObjectRx, MSG_OBJ_TYPE_RX);
    printf("setToMachine SetMessage: %s\n", g_MsgObjectRx.pucMsgData);
    while((CANStatusGet(CAN0_BASE, CAN_STS_NEWDAT)) & 1 == 0)
    {}
		CANMessageGet(CAN0_BASE, 1, &g_MsgObjectRx, true);
		printf("setToMachine Message: %s\n", g_MsgObjectRx.pucMsgData);
    
	
	return rc;
}


int getFormMachine(char* id)
{
	int value = -999; // error code
	
		int rc = 0;
	
	g_MsgObjectRx.ulMsgID = (0x400);
    g_MsgObjectRx.ulMsgIDMask = 0x7f8;        
    g_MsgObjectRx.ulFlags = MSG_OBJ_USE_ID_FILTER;
    g_MsgObjectRx.ulMsgLen = snprintf(ucBufferIn, BUFFER_LEN, "%s", id, value);            
    g_MsgObjectRx.pucMsgData = ucBufferIn;
	
	CANMessageSet(CAN0_BASE, 1, &g_MsgObjectRx, MSG_OBJ_TYPE_RX);
    
	printf("getFormMachine Message: %s\n", g_MsgObjectRx.pucMsgData);
	
    while((CANStatusGet(CAN0_BASE, CAN_STS_NEWDAT)) & 1 == 0)
    {
    }
	
	CANMessageGet(CAN0_BASE, 1, &g_MsgObjectRx, true);
		value = atoi(strstr(g_MsgObjectRx.pucMsgData) + 1);
		printf("getFromMachine GetMessage: %s\n", g_MsgObjectRx.pucMsgData);
	
	return rc;
	
	return value;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

