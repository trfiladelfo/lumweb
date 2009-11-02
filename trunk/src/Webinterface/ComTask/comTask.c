/**
 * \addtogroup comTask
 * @{
 *
 * \file comTask.c
 * \author Anziner, Hahn
 * \brief implements the Communication Task (vComTask) to interact with the machine
 *
 *
 */

/*
 * $Rev$ $Author$
 * $LastChangedDate$
 *
 * GPV v3  (c) 2009
 *
 */

/* queue includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* HW includes */
#include "portmacro.h"

/* Include Queue staff */
#include "comTask.h"
#include "GraphicsLibrary/graphicObjects.h"
#include "webserver/httpd-queue.h"
#include "fs.h"

void vComTask(void *pvParameters)
{
	xCOMMessage xMessage;
	xGraphCommunication xGraph_msg;
	xHTTPDMessage xHTTPD_msg;

	int value;

	int day_hour = 10;
	int day_minute = 30;
	int night_hour = 23;
	int night_minute = 15;

	//FILE_HANDLE mainfile;
	char msg[30];

	//FSInit();
	//mainfile = FSOpen("main.txt\0");
	//if (mainfile != NULL) {
	//	FSRead(mainfile, msg, 30);

	//xGraph_msg.msg = msg;
	//} else {
	//	xGraph_msg.msg = "READ ERROR!";
	//}

	//xQueueSend(xGRAPHQueue, &xGraph_msg, (portTickType) 0);

	for (;;)
	{
		/* Wait for a message to arrive */
		if (xQueueReceive( xCOMQueue, &xMessage, ( portTickType ) 1000 )
				== pdTRUE)
		{
			if (xMessage.cmd == GET)
			{
				if (strcmp(xMessage.item, "day_hour") == 0)
				{
					sprintf(msg, "\"%d\"", day_hour);
					value = day_hour;
				}
				else if (strcmp(xMessage.item, "day_minute") == 0)
				{
					sprintf(msg, "\"%d\"", day_minute);
					value = day_minute;
				}
				else if (strcmp(xMessage.item, "night_hour") == 0)
				{
					sprintf(msg, "\"%d\"", night_hour);
					value = night_hour;
				}
				else if (strcmp(xMessage.item, "night_minute") == 0)
				{
					sprintf(msg, "\"%d\"", night_minute);
					value = night_minute;
				}
				else
				{
					sprintf(msg, "\"%s: %s\"", "ERROR", xMessage.item);
				}

				// Antwort senden
				if (xMessage.from == HTTPD)
				{
					xHTTPD_msg.msg = msg;
					xQueueSend(xHTTPDQueue, &xHTTPD_msg, (portTickType) 0);
				}
				else if (xMessage.from == GRAPHIC)
				{
					xGraph_msg.value = value;
					xQueueSend(xGraphCommunicationQueue, &xGraph_msg, (portTickType) 0);
					vTaskResume(xGraphicObjectsTaskHandler);
				}
			}
			else if (xMessage.cmd == SET)
			{
				if (strcmp(xMessage.item, "day_hour") == 0)
				{
					day_hour = (int) (xMessage.value);
				}
				else if (strcmp(xMessage.item, "day_minute") == 0)
				{
					day_minute = (int) (xMessage.value);
				}
				else if (strcmp(xMessage.item, "night_hour") == 0)
				{
					night_hour = (int) (xMessage.value);
				}
				else if (strcmp(xMessage.item, "night_minute") == 0)
				{
					night_minute = (int) (xMessage.value);
				}
			}
		}

		// send can bus message */
	}
}
