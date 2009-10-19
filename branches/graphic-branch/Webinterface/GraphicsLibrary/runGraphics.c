/**
 * Start Library for the Graphics Task
 * @author anzinger
 * @version 1.0/14-10-09
 */

#include "GraphicsLibrary/graphicObjects.h"
#include "GraphicsLibrary/runGraphics.h"

// Testroutine
void vActionButton1(void) {

}
// Testroutine
void vActionButton2(void) {

}
// Testroutine
void vActionButton3(void) {

}
// Testroutine
void vActionButton4(void) {

}

void vGraphicObjectsTask(void *pvParameters) {
	//pgoButton b;
	//pgoButton b1;

	//pgoButton b2;
	//pgoButton b3;

	goInit();

	//b = goNewButton(10, 10, 10, 10, goButtonUp, vActionButton1);
	//b2 = goNewButton(10, 10, 24, 10, goButtonDown, vActionButton3);

	//b1 = goNewButton(10, 10, 30, 10, goButtonUp, vActionButton2);
	//b3 = goNewButton(10, 10, 30, 30, goButtonDown, vActionButton4);

	goStartListener(xGraphicTaskHandler);

	while (1) {
		// ENDLESS LOOP!
	}
}
