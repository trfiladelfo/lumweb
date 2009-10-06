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


void vComTask( void *pvParameters )
{
	xCOMMessage xMessage;
	xGRAPHMessage xGraph_msg;
	char msg[] = "Hallo";

	xGraph_msg.msg = msg;
	for( ;; )
    {
		/* Wait for a message to arrive */
		xQueueReceive( xCOMQueue, &xMessage,  ( portTickType ) 10 );
		//xQueueSend(xGRAPHQueue, &xGraph_msg, ( portTickType ) 0);
		// send can bus message
    }
}
