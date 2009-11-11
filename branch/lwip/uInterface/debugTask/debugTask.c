/*
 * debugUART.c
 *
 *  Created on: 06.10.2009
 *      Author: anzinger
 */

#include <string.h>

#include "debugTask.h" /* include the debugging task */

#include "FreeRTOS.h"
#include "queue.h"
//#include "task.h"

//#include "psock.h"
//#include "uip.h"

#include "GraphicsLibrary/graphicObjects.h"

int initStatus = 0;

//*****************************************************************************
//
// Send a string to the Debug Terminal.
//
//*****************************************************************************
void vSendDebug(unsigned char *pucBuffer)
{
	//vInitDebug();

	xGraphMessage xMessage;
	//xDebugTaskMessage xDebugMessage;

	xMessage.msg = pucBuffer;
	//xDebugMessage.msg = pucBuffer;

	//xQueueSend(xDebugQueue, &xDebugMessage, (portTickType) 0);
	//vTaskResume(xDebugQueue);

	xQueueSend(xGraphQueue, &xMessage, (portTickType) 0);
	vTaskResume(xGraphicObjectsTaskHandler);
}

//*****************************************************************************
//
// Task which sends the Debug-Infos to a Telnet session
//
//*****************************************************************************
void vDebugTask(void *pvParameter)
{
	xDebugTaskMessage xMessage;

	vInitDebug();
	//vInitDebugTask(DEBUG_PORT);

	while (xQueueReceive(xDebugQueue, &xMessage, (portTickType) 100))
	{

	}

}

//*****************************************************************************
//
// Initializes the Task
//
//*****************************************************************************
void vInitDebugTask(int port)
{
	vInitDebug();
	//uip_listen(HTONS(port));
}

void vInitDebug(void)
{
	if (!initStatus)
	{
		xDebugQueue = xQueueCreate(5, sizeof(xDebugTaskMessage));
		initStatus = 1;
	}
}
