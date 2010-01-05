/**
 * This is the Graphics Task for the LM3S9B96 Board
 */

#include "graphic/graphicTask.h"
#include "graphic/graphicLib.h"
//#include "graphic/settings/settings.h"

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


void setIP(tWidget *pWidget) {
	/*
	 //
	 // Remove the current panel.
	 //
	 WidgetRemove(pWidget);

	 //
	 // Add and draw the new panel.
	 //
	 WidgetAdd(WIDGET_ROOT, (tWidget *) (&g_psSliderPanels));

	 //
	 // Issue the initial paint request to the widgets.
	 //
	 WidgetPaint(WIDGET_ROOT);*/
	printf("Setze IP aufgerufen\n");

}

void sliderChange (tWidget *pWidget, long value) {
	printf("neuer Slider wert: %d\n", value);
}

void vGraphicTask(void* pvParameters) {

	printf("Initialize Graphic ...\n");

	initPanel("Hallo Welt!!!");

	addButton(285, 40, 30, 30, "U", setIP);
	addButton(285, 205, 30, 30, "D", setIP);

	addButton(120, 40, 30, 30, "-", setIP);
	addSlider(155, 40, 80, 30, "Wert1", 50, sliderChange);
	addButton(240, 40, 30, 30, "+", setIP);
	addButton(120, 75, 30, 30, "-", setIP);
	addSlider(155, 75, 80, 30, "Wert2", 50, sliderChange);
	addButton(240, 75, 30, 30, "+", setIP);
	drawPanel();
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

