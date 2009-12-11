/**
 * This is the Graphics Task for the LM3S9B96 Board
 */

#include "graphicTask.h"
#include "hw_types.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "graphic/settings/settings.h"
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "lwip/ip_addr.h"

//*****************************************************************************
//
// The sixth panel, which contains a selection of push buttons.
//
//*****************************************************************************

extern tCanvasWidget g_psPanels[];

tCanvasWidget g_psPushButtonIndicators[] = {

CanvasStruct(&g_psPanels, g_psPushButtonIndicators + 1, 0,
		&g_sKitronix320x240x16_SSD2119, 50, 30, 20, 20,
		CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, 0, 0),

CanvasStruct(&g_psPanels, g_psPushButtonIndicators + 2, 0,
		&g_sKitronix320x240x16_SSD2119, 150, 30, 20, 20,
		CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, 0, 0),

CanvasStruct(&g_psPanels, 0, 0,
		&g_sKitronix320x240x16_SSD2119, 250, 30, 20, 20,
		CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, 0, 0)

};

tPushButtonWidget
		g_psPushButtons[] = {
								RectangularButtonStruct(&g_psPanels, g_psPushButtons + 1, 0, &g_sKitronix320x240x16_SSD2119, 30, 35,260, 40, PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT, ClrMidnightBlue, ClrBlack, ClrGray, ClrSilver, &g_sFontCm22, "Setze IP", 0, 0, 0, 0, setIP),
								RectangularButtonStruct(&g_psPanels, g_psPushButtons + 2, 0, &g_sKitronix320x240x16_SSD2119, 30, 85, 260, 40, PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT, ClrMidnightBlue, ClrBlack, ClrGray, ClrSilver, &g_sFontCm22, "Setze Gateway", 0, 0, 0, 0, setGW),
								RectangularButtonStruct(&g_psPanels, 0, 0, &g_sKitronix320x240x16_SSD2119, 30, 135, 260, 40, PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT, ClrMidnightBlue, ClrBlack, ClrGray, ClrSilver, &g_sFontCm22, "Setze Netmask", 0, 0, 0, 0, setNETMASK) };

tCanvasWidget
		g_psPanels[] = {
								CanvasStruct(0, 0, g_psPushButtons, &g_sKitronix320x240x16_SSD2119, 0, 30, 320, 166, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0)

		};

void vGraphicTask(void* pvParameters) {

	printf("Initialize Graphic ...\n");
	tCanvasWidget widget = CanvasStruct(0, 0, g_psPushButtons,
			&g_sKitronix320x240x16_SSD2119, 0, 24, 320, 166, CANVAS_STYLE_FILL,
			ClrBlack, 0, 0, 0, 0, 0, 0);
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

	printf("Drawing ROOT Widget ...\n");
	WidgetAdd(WIDGET_ROOT, (tWidget *) (&g_psPanels));

	//
	// Issue the initial paint request to the widgets.
	//
	WidgetPaint(WIDGET_ROOT);

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

