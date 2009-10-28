/**
 * Start Library for the Graphics Task
 * @author anzinger
 * @version 1.0/14-10-09
 */

#include "GraphicsLibrary/graphicObjects.h"
#include "GraphicsLibrary/graphicTextbox.h"
#include "GraphicsLibrary/graphicButton.h"
#include "GraphicsLibrary/runGraphics.h"

void vGraphicObjectsTask(void *pvParameters) {
	pgoButton b;
	pgoButton b1;
	pgoButton b2;
	pgoButton b3;

	pgoTextBox t;
	pgoTextBox t1;

	unsigned char* dt = {"day_hour"};
	unsigned char* dt1 = {"day_minute"};

	goInit();

	t = goNewTextBox(7, 50, 10, dt);
	t1 = goNewTextBox(7, 50, 24, dt1);

	b = goNewButton(10, 10, 114, 10, goButtonUp, pucBorderNormal,
			vTextBoxIncrement);
	b1 = goNewButton(10, 10, 114, 24, goButtonDown, pucBorderNormal,
			vTextBoxDecrement);

	goObjectsListener(xGraphicTaskHandler);

	while (1) {
		// ENDLESS LOOP!
	}
}

void vScreenSaver(void) {
	goDisplaySleep();
}
