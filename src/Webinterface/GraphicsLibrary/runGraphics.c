/**
 * Start Library for the Graphics Task
 * @author anzinger
 * @version 1.0/14-10-09
 */

#include "GraphicsLibrary/graphicObjects.h"
#include "GraphicsLibrary/runGraphics.h"

void vGraphicObjectsTask(void *pvParameters) {
	pgoButton b;
	pgoButton b1;
	pgoButton b2;
	pgoButton b3;

	pgoTextBox t;
	pgoTextBox t1;

	unsigned int dt = 10;
	unsigned int dt1 = 50;

	goInit();

	t = goNewTextBox(7, 40, 11, &dt);
	t1 = goNewTextBox(7, 40, 31, &dt1);

	b = goNewButton(10, 10, 10, 10, goButtonUp, pucBorderNormal,
			vTextBoxIncrement, t);
	b1 = goNewButton(10, 10, 24, 10, goButtonDown, pucBorderNormal,
			vTextBoxDecrement, t);

	b2 = goNewButton(10, 10, 10, 30, goButtonUp, pucBorderNormal,
			vTextBoxIncrement, t1);
	b3 = goNewButton(10, 10, 24, 30, goButtonDown, pucBorderNormal,
			vTextBoxDecrement, t1);

	goObjectsListener(xGraphicTaskHandler);

	while (1) {
		// ENDLESS LOOP!
	}
}

void vScreenSaver(void) {
	goDisplaySleep();
}
