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

#include "comTask.h"
#include  "GraphicsLibary/graphicObjects.h"

#include "fs.h"

void vComTask(void *pvParameters) {
	xCOMMessage xMessage;
	xGRAPHMessage xGraph_msg;
	FILE_HANDLE mainfile;
	char msg[30];

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
		xQueueReceive( xCOMQueue, &xMessage, ( portTickType ) 10 );
		// send can bus message
	}
}
