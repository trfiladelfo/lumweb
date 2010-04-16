/**
 * \addtogroup comTask
 * @{
 *
 * \author Anziner, Hahn
 * \brief implements the Communication Task (vComTask) to interact with the machine
 *
 *
 */

/* std lib includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* HW includes */
#include "portmacro.h"
#include "hw_memmap.h"
#include "hw_types.h"

/* Include Queue staff */
#include "comTask.h"
#include "taskConfig.h"
#include "queueConfig.h"

#include "setup.h"

xComMessage xMessage;

/* Testvalues are read from sd card ! */

void vComTask(void *pvParameters)
{
	char buffer[100];

	vComTaskInitImpl();

	for (;;)
	{
		/* Wait for a message to arrive */
		if (xQueueReceive( xComQueue, &xMessage, ( portTickType ) 100 )
				== pdTRUE)
		{

#if DEBUG_COM
			printf("ComTask: Got Item from Queue \n");
#endif

			xMessage.errorDesc = NULL;

			if (xMessage.cmd == GET)
			{
				xMessage.value = -999;

				xMessage.value = getFormMachine(xMessage.item);

				if (xMessage.value == -999)
					sprintf(xMessage.errorDesc, "\"ERROR: %s\"", xMessage.item);

#if DEBUG_COM
				printf("COMTASK: Sende wert zurueck (%s, %d)\n", xMessage.item,
						xMessage.value);
#endif
				xQueueSend(xHttpdQueue, &xMessage, (portTickType) 0);
				vTaskResume(xMessage.taskToResume);

			}
			else if (xMessage.cmd == SET)
			{

				if (sendToMachine(xMessage.item, xMessage.value) == -1)
					sprintf(buffer, "FAIL: %s", xMessage.item);

#if DEBUG_COM
				printf("COMTASK: Daten gespeichert (%s = %d)\n", xMessage.item,
						xMessage.value);
#endif

				if (xMessage.freeItem == pdTRUE)
				{
					vPortFree(xMessage.item);
				}
				vTaskResume(xMessage.taskToResume);
			}
		}
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

