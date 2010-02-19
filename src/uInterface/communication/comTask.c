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
#include <stdlib.h>

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

/* FatFs includes */
#include "fatfs/ff.h"
#include "lmi_fs.h"


/* Include Queue staff */
#include "comTask.h"
#include "../taskConfig.h"
#include "../queueConfig.h"
#include "../uart/uartstdio.h"


#define PATH_TO_DATA	"data/"

xComMessage xMessage;

char path_buf[32], buf[32];
FIL save_file;

/* Testvalues are read from sd card ! */

int sendToMachine(char* id, int value){
	unsigned int bw;
	int rc = 0;

	// suspend all other tasks
	vTaskSuspendAll();

	fs_enable(400000);

	strcat(path_buf, PATH_TO_DATA);
	strncat(path_buf, id, 8);

	printf("SendToMachine: opening file: %s \n", path_buf);

	rc = f_open(&save_file, path_buf, FA_CREATE_NEW);

	if(rc = FR_EXIST)
		rc = f_open(&save_file, path_buf, FA_WRITE);

	if(rc == FR_OK){
		sprintf(buf, "%d", value);
		rc = f_write(&save_file, &buf, strlen(buf), &bw);
		printf("SendToMachine: rc: %d - wrote '%s' to file\n",rc, buf);
		f_sync(&save_file);
		f_close(&save_file);
	}else
		printf("SendToMachine: Error opening file, rc=%d\n", rc);

	path_buf[0] = 0;
	buf[0] = 0;

	// resumes all tasks
	xTaskResumeAll();

/* CAN Test Stuff
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
*/

	return rc;
}

int getFormMachine(char* id){
	int value = -999; // error code
	int rc;

	// suspend all other tasks
	vTaskSuspendAll();

	fs_enable(400000);

	strcat(path_buf, PATH_TO_DATA);
	strncat(path_buf, id, 8);

	printf("getFormMachine: opening file: '%s' \n", path_buf);
	rc = f_open(&save_file, path_buf, FA_READ);

	if(rc == FR_OK){
		f_gets(&buf, 32, &save_file);
		value = atoi(buf);
		printf("getFormMachine: rc: %d - read '%d' from file\n",rc, value);
		f_close(&save_file);
	}else
		printf("getFormMachine: Error opening file, rc=%d\n", rc);

	path_buf[0] = 0;
	buf[0] = 0;

	// resumes all tasks
	xTaskResumeAll();

	return value;
}

void vComTask(void *pvParameters) {
	char buffer[100];
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
	}
}
