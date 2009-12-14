/**
 * This is the Graphics Task for the LM3S9B96 Board
 */

#include "graphicTask.h"
#include "hw_types.h"
#include "grlib/grlib.h"
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "lwip/ip_addr.h"

//*****************************************************************************
//
// The sixth panel, which contains a selection of push buttons.
//
//*****************************************************************************


void vGraphicTask(void* pvParameters) {

	printf("Initialize Graphic ...\n");
	tRectangle sRect;


	//
	// Initialize the graphics context.
	//
	printf("Initialize Graphic Context ...\n");
	GrContextInit(&g_sContext, &g_sKitronix320x240x16_SSD2119);

	//
	// Fill the top 24 rows of the screen with blue to create the banner.
	//
	sRect.sXMin = 5;
	sRect.sYMin = 5;
	sRect.sXMax = GrContextDpyWidthGet(&g_sContext) - 5;
	sRect.sYMax = 28;
	GrContextForegroundSet(&g_sContext, ClrDarkBlue);
	GrRectFill(&g_sContext, &sRect);

	//
	// Put a white box around the banner.
	//
	GrContextForegroundSet(&g_sContext, ClrWhite);
	GrRectDraw(&g_sContext, &sRect);

	//
	// Put the application name in the middle of the banner.
	//
	GrContextFontSet(&g_sContext, &g_sFontCm12b);
	GrStringDrawCentered(&g_sContext, "Luminary Touchinterface", -1,
			GrContextDpyWidthGet(&g_sContext) / 2, 15, 0);

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

