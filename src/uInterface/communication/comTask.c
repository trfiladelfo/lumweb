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
 * $Rev: 146 $ $Author: martin.anzinger $
 * $LastChangedDate: 2009-11-11 11:52:35 +0100 (Mi, 11 Nov 2009) $
 *
 * GPV v3  (c) 2009
 *
 */
#include <stdio.h>

/* queue includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* HW includes */
#include "portmacro.h"

/* Include Queue staff */
#include "comTask.h"
#include "../taskConfig.h"
#include "../queueConfig.h"
#include "../uart/uartstdio.h"

/* TODO only testvalues */
int day_hour = 10;
int day_minute = 30;
int night_hour = 23;
int night_minute = 15;

xComMessage xMessage;

void vComTask(void *pvParameters) {
	char buffer[100];

	for (;;) {
		/* Wait for a message to arrive */
		if (xQueueReceive( xComQueue, &xMessage, ( portTickType ) 100 )
				== pdTRUE) {

			printf("ComTask: Got Item from Queue \n");

			xMessage.errorDesc = NULL;

			if (xMessage.cmd == GET) {
				xMessage.value = -999;

				if (strcmp(xMessage.item, "day_hour") == 0) {
					xMessage.value = day_hour;
				} else if (strcmp(xMessage.item, "day_minute") == 0) {
					xMessage.value = day_minute;
				} else if (strcmp(xMessage.item, "night_hour") == 0) {
					xMessage.value = night_hour;
				} else if (strcmp(xMessage.item, "night_minute") == 0) {
					xMessage.value = night_minute;
				} else {
					sprintf(xMessage.errorDesc, "\"ERROR: %s\"", xMessage.item);
				}

				printf("COMTASK: Sende wert zurueck (%s, %d)\n", xMessage.item,
						xMessage.value);
				xQueueSend(xHttpdQueue, &xMessage, (portTickType) 0);
				vTaskResume(xMessage.taskToResume);

			} else if (xMessage.cmd == SET) {

				if (strcmp(xMessage.item, "day_hour") == 0) {
					day_hour = xMessage.value;
				} else if (strcmp(xMessage.item, "day_minute") == 0) {
					day_minute = xMessage.value;
				} else if (strcmp(xMessage.item, "night_hour") == 0) {
					night_hour = xMessage.value;
				} else if (strcmp(xMessage.item, "night_minute") == 0) {
					night_minute = xMessage.value;
				} else {
					sprintf(buffer, "FAIL: %s", xMessage.item);
					printf("COMTASK: %s\n", buffer);
				}
					printf("COMTASK: Daten gespeichert (%s = %d)\n", xMessage.item,
							xMessage.value);

				if (xMessage.freeItem == pdTRUE) {
					vPortFree(xMessage.item);
				}
				vTaskResume(xMessage.taskToResume);
			}

		}
		// send can bus message */
	}
}
