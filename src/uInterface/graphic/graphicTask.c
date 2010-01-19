/**
 * This is the Graphics Task for the LM3S9B96 Board
 */

#include "graphic/graphicTask.h"
#include "graphic/graphicWebClient.h"
#include "graphic/graphicLib.h"

#include "hw_types.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"

#include "kitronix320x240x16_ssd2119_8bit.h"


//*****************************************************************************
//
// The sixth panel, which contains a selection of push buttons.
//
//*****************************************************************************

void vGraphicTask(void* pvParameters) {

	printf("Initialize Graphic ...\nLoad Menu\n");

	showBootText("booting ...");

	//
	// Loop forever handling widget messages.
	//
	printf("Waiting for Touch Interrups ...\n");
	while (1) {
		//
		// Process any messages in the widget message queue.
		//
		WidgetMessageQueueProcess();
		vTaskDelay(1);
	}
}
