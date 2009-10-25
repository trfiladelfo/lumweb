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
#include  "GraphicsLibrary/graphicObjects.h"
#include "webserver/httpd-queue.h"

#include "fs.h"

void vComTask(void *pvParameters) {
	xCOMMessage xMessage;
	xGraphMessage xGraph_msg;
	xHTTPDMessage xHTTPD_msg;

	int day_hour = 14, day_minute = 0, night_hour = 22,
			night_minute = 1;

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
		if (xQueueReceive( xCOMQueue, &xMessage, ( portTickType ) 1000 )
				== pdTRUE) {

			if (xMessage.cmd == GET) {
				if (strcmp(xMessage.item, "day_hour") == 0) {
					sprintf(msg, "\"%d\"", day_hour);
				} else if (strcmp(xMessage.item, "day_minute") == 0) {
					sprintf(msg, "\"%d\"", day_minute);
				} else if (strcmp(xMessage.item, "night_hour") == 0) {
					sprintf(msg, "\"%d\"", night_hour);
				} else if (strcmp(xMessage.item, "night_minute") == 0) {
					sprintf(msg, "\"%d\"", night_minute);
				} else
					sprintf(msg, "\"%s: %s\"", "ERROR",
							xMessage.item);

				if (xMessage.from == HTTPD) {
					xHTTPD_msg.msg = msg;
					xQueueSend(xHTTPDQueue, &xHTTPD_msg, (portTickType) 0);
				}
				if (xMessage.from == GRAPHIC) {
					//xHTTPD_msg.msg = msg;
					//xQueueSend(xGraphCommTaskQueue, &xGraph_msg, (portTickType) 0);
				}
			} else if(xMessage.cmd == SET){
				xGraph_msg.msg = "SET";
				xQueueSend(xGraphQueue, &xGraph_msg, (portTickType) 0);
				if(xMessage.item == "day_hour"){
					day_hour = atoi(xMessage.value);
				}
			}

		}
		// send can bus message */
	}
}
