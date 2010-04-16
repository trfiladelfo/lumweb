/**
 * \addtogroup Graphic
 * @{
 *
 * \author Anziner, Hahn
 * \brief Implementation of Graphics Task for the LM3S9B96 Board
 *
 */

#include "FreeRTOS.h"
#include "task.h"

#include "graphic/graphicTask.h"
#include "graphic/gui/displayBasics.h"

//*****************************************************************************
//
// Task for Initializing the Display and handels the Touch Interrupts
//
//*****************************************************************************

void vGraphicTask(void* pvParameters)
{

	vInitDisplay();
	vShowBootText("booting ...");

	while (1)
	{
		//
		// Process any messages in the widget message queue.
		//
		WidgetMessageQueueProcess();
		vTaskDelay(1);
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

