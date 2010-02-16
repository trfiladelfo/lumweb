/*
 * $Rev: 146 $ $Author: martin.anzinger $
 * $LastChangedDate: 2009-11-11 11:52:35 +0100 (Mi, 11 Nov 2009) $
 *
 * GPV v3  (c) 2009
 *
 */

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

#include <stdio.h>
#include <string.h>

/* queue includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* HW includes */
#include "portmacro.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_can.h"
#include "can.h"

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
int checkbox = 1;
int heizkurve = 1;
int norm_temp = 21;
int abs_temp = 17;


xComMessage xMessage;

int sendToMachine(char* id, int value){
	int rc = 0;

	if (strcmp(id, "day_hour") == 0) {
		day_hour = xMessage.value;
	} else if (strcmp(id, "day_minute") == 0) {
		day_minute = value;
	} else if (strcmp(id, "night_hour") == 0) {
		night_hour = value;
	} else if (strcmp(id, "night_minute") == 0) {
		night_minute = value;
	} else if (strcmp(id, "heizkurve") == 0) {
		heizkurve = value;
	} else if (strcmp(id, "led_ein") == 0) {
		checkbox = value;
	} else if (strcmp(id, "norm_temp") == 0) {
		norm_temp = value;
	} else if (strcmp(id, "abs_temp") == 0) {
		abs_temp = value;
	} else {
		rc = -1;
	}

	return rc;
}

int getFormMachine(char* id){
	int value = -999; // error code
	if (strcmp(id, "day_hour") == 0) {
		value = day_hour;
	} else if (strcmp(id, "day_minute") == 0) {
		value = day_minute;
	} else if (strcmp(id, "night_hour") == 0) {
		value = night_hour;
	} else if (strcmp(id, "night_minute") == 0) {
		value = night_minute;
	} else if (strcmp(id, "led_ein") == 0) {
		value = checkbox;
	} else if (strcmp(id, "norm_temp") == 0) {
		value = norm_temp;
	} else if (strcmp(id, "abs_temp") == 0) {
		value = abs_temp;
	}else if (strcmp(id, "heizkurve") == 0) {
		value = heizkurve;
	}

	return value;
}

void vComTask(void *pvParameters) {
	char buffer[100];
	char ucBufferOut[8];
	tCANMsgObject sMsgObjectTx;

	strcpy(ucBufferOut, "TEST");

	//
	// Configure and start transmit of message object.
	//
	sMsgObjectTx.ulMsgID = 0x400;
	sMsgObjectTx.ulFlags = 0;
	sMsgObjectTx.ulMsgLen = 8;
	sMsgObjectTx.pucMsgData = (unsigned char*) ucBufferOut;
	CANMessageSet(CAN0_BASE, 2, &sMsgObjectTx, MSG_OBJ_TYPE_TX);

	for (;;) {
		/* Wait for a message to arrive */
		if (xQueueReceive( xComQueue, &xMessage, ( portTickType ) 100 )
				== pdTRUE) {

			//printf("ComTask: Got Item from Queue \n");

			xMessage.errorDesc = NULL;

			if (xMessage.cmd == GET) {
				xMessage.value = -999;

				xMessage.value = getFormMachine(xMessage.item);

				if(xMessage.value == -999)
					sprintf(xMessage.errorDesc, "\"ERROR: %s\"", xMessage.item);


				//printf("COMTASK: Sende wert zurueck (%s, %d)\n", xMessage.item,
				//		xMessage.value);
				xQueueSend(xHttpdQueue, &xMessage, (portTickType) 0);
				vTaskResume(xMessage.taskToResume);

			} else if (xMessage.cmd == SET) {

				if(sendToMachine(xMessage.item, xMessage.value) == -1)
					sprintf(buffer, "FAIL: %s", xMessage.item);

				//printf("COMTASK: Daten gespeichert (%s = %d)\n", xMessage.item,
				//		xMessage.value);

				if (xMessage.freeItem == pdTRUE) {
					vPortFree(xMessage.item);
				}
				vTaskResume(xMessage.taskToResume);
			}
		}
		// send can bus message */
	}
}
