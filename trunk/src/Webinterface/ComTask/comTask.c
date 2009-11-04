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

/* TODO only testvalues */
int day_hour = 10;
int day_minute = 30;
int night_hour = 23;
int night_minute = 15;

xCOMMessage xMessage;

void vComTask(void *pvParameters)
{
	char buffer[100];

	for (;;)
	{
		/* Wait for a message to arrive */
		if (xQueueReceive( xCOMQueue, &xMessage, ( portTickType ) 100 )
				== pdTRUE && xMessage.dataSouce == DATA)
		{
			xMessage.errorDesc = NULL;

			if (xMessage.cmd == GET)
			{
				xMessage.value = -999;

				if (strcmp(xMessage.item, "day_hour") == 0)
				{
					xMessage.value = day_hour;
				}
				else if (strcmp(xMessage.item, "day_minute") == 0)
				{
					xMessage.value = day_minute;
				}
				else if (strcmp(xMessage.item, "night_hour") == 0)
				{
					xMessage.value = night_hour;
				}
				else if (strcmp(xMessage.item, "night_minute") == 0)
				{
					xMessage.value = night_minute;
				}
				else
				{
					sprintf(xMessage.errorDesc, "\"ERROR: %s\"", xMessage.item);
				}

				xQueueSend(xMessage.from, &xMessage, (portTickType) 0);

				//vSendDebugUART("Daten gesendet");

			}
			else if (xMessage.cmd == SET)
			{
				sprintf(buffer, "%s=%d",xMessage.item, xMessage.value);
				vSendDebug(buffer);
				//vSendDebugUART("Daten gespeichert");
				if (strcmp(xMessage.item, "day_hour") == 0)
				{
					day_hour = xMessage.value;
				}
				else if (strcmp(xMessage.item, "day_minute") == 0)
				{
					day_minute = xMessage.value;
				}
				else if (strcmp(xMessage.item, "night_hour") == 0)
				{
					night_hour = xMessage.value;
				}
				else if (strcmp(xMessage.item, "night_minute") == 0)
				{
					night_minute = xMessage.value;
				} else {
					sprintf(buffer, "FAIL: %s", xMessage.item);
					vSendDebug(buffer);
				}

				if(xMessage.freeItem == pdTRUE){
					vPortFree(xMessage.item);
				}
			}

			if (xMessage.taskToResume != NULL)
			{
				vTaskResume(xMessage.taskToResume);
			}
		}

		// send can bus message */
	}
}
