/*
 * debugUART.c
 *
 *  Created on: 06.10.2009
 *      Author: anzinger
 */

#include "debugUART.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "gpio.h"
#include "sysctl.h"
#include "uart.h"
#include "string.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "GraphicsLibrary/graphicObjects.h"
#include "GraphicsLibrary/runGraphics.h"

int init = 0;

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void vSendDebugUART(unsigned char *pucBuffer)
{
	xGraphMessage xMessage;
	xMessage.msg = pucBuffer;
	xQueueSend(xGraphQueue, &xMessage, (portTickType) 0);
	xTaskResumeFromISR(xGraphicTaskHandler);
}
