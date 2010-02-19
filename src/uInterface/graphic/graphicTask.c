/**
 * \addtogroup Graphic
 * @{
 *
 * \file graphicTask.c
 * \author Anziner, Hahn
 * \brief Implementation of Graphics Task for the LM3S9B96 Board
 *
*/

#include "FreeRTOS.h"
#include "task.h"

#include "graphic/graphicTask.h"
#include "graphic/gui/dislpayBasics.h"

//*****************************************************************************
//
// Task for Initializing the Display and handels the Touch Interrupts
//
//*****************************************************************************

void vGraphicTask(void* pvParameters) {

	vInitDisplay();
	vShowBootText("booting ...");

	while (1) {
		//
		// Process any messages in the widget message queue.
		//
		WidgetMessageQueueProcess();
		vTaskDelay(1);
	}
}
