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

/* HW includes */
#include "portmacro.h"

/* Include Queue staff */
#include "comTask.h"
#include  "GraphicsLibary/graphicObjects.h"
#include "webserver/httpd-queue.h"

#include "fs.h"

void vComTask(void *pvParameters) {
	xCOMMessage xMessage;
	xGRAPHMessage xGraph_msg;
	xHTTPDMessage xHTTPD_msg;

	FILE_HANDLE mainfile;
	char *msg;

	msg = pvPortMalloc(30);

	FSInit();
	mainfile = FSOpen("main.txt\0");
	if (mainfile != NULL) {
		FSRead(mainfile, msg, 30);

		xGraph_msg.msg = msg;

	} else {
		xGraph_msg.msg = "READ ERROR!";
	}

	//xQueueSend(xGRAPHQueue, &xGraph_msg, (portTickType) 0);

	for (;;) {
		/* Wait for a message to arrive */
		if(xQueueReceive( xCOMQueue, &xMessage, ( portTickType ) 1000 ) == pdTRUE){
			if(xMessage.cmd == GET){
				if(strcmp(xMessage.item, "day_hour") == 0){
					sprintf(msg, "\"%d\"", 10);
				} else if(strcmp(xMessage.item, "day_minute") == 0){
					sprintf(msg, "\"%d\"", 11);
				} else if(strcmp(xMessage.item, "night_hour") == 0){
					sprintf(msg, "\"%d\"", 12);
				} else if(strcmp(xMessage.item, "night_minute") == 0){
					sprintf(msg, "\"%d\"", 13);
				} else
					sprintf( xHTTPD_msg.msg, "\"%s: %s\"", "ERROR", xMessage.item);
			}
			if(xMessage.from == HTTPD){
			//	xGraph_msg.msg = msg;
			//	xQueueSend(xGRAPHQueue, &xGraph_msg, (portTickType) 0);
				xHTTPD_msg.msg = msg;
				xQueueSend(xHTTPDQueue, &xHTTPD_msg, (portTickType) 0);
			}
		}
			// send can bus message */
	}
}
