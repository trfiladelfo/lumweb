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

/*
 * Declaration of the protosocket function that handles the connection
 * (defined at the end of the code).
 */
//static int handle_connection(struct hello_world_state *s);

/*---------------------------------------------------------------------------*/
/*
 * In hello-world.h we have defined the UIP_APPCALL macro to
 * hello_world_appcall so that this funcion is uIP's application
 * function. This function is called whenever an uIP event occurs
 * (e.g. when a new connection is established, new data arrives, sent
 * data is acknowledged, data needs to be retransmitted, etc.).
 */
//void debug_appcall(void)
//{
/*
 * The uip_conn structure has a field called "appstate" that holds
 * the application state of the connection. We make a pointer to
 * this to access it easier.
 */
//struct debug_state *s = &(uip_conn->appstate);

/*
 * If a new connection was just established, we should initialize
 * the protosocket in our applications' state structure.
 */
/*if (uip_connected())
 {
 PSOCK_INIT(&s->p, s->inputbuffer, sizeof(s->inputbuffer));
 }*/

/*
 * Finally, we run the protosocket function that actually handles
 * the communication. We pass it a pointer to the application state
 * of the current connection.
 */
//handle_connection(s);
//}
/*---------------------------------------------------------------------------*/
/*
 * This is the protosocket function that handles the communication. A
 * protosocket function must always return an int, but must never
 * explicitly return - all return statements are hidden in the PSOCK
 * macros.
 */
//static int handle_debug_connection(struct debug_state *s)
//{
/*PSOCK_BEGIN(&s->p);

 PSOCK_SEND_STR(&s->p, "Hello. What is your name?\n");
 PSOCK_READTO(&s->p, '\n');
 strncpy(s->name, s->inputbuffer, sizeof(s->name));
 PSOCK_SEND_STR(&s->p, "Hello ");
 PSOCK_SEND_STR(&s->p, s->name);
 PSOCK_CLOSE(&s->p);

 PSOCK_END(&s->p);*/
//}
