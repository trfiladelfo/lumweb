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

void vComTask( void *pvParameters )
{
	xCOMMessage xMessage;

	for( ;; )
    {
		/* Wait for a message to arrive */
		xQueueReceive( xCOMQueue, &xMessage, portMAX_DELAY );

		// send can bus message
    }
}
