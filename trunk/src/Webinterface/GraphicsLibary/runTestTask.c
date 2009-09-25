/*
 * graphicObjects.c
 *
 *  Created on: 23.09.2009
 *      Author: anzinger
 *
 * This File is used to Run the the Graphic Objects Libary.
 *
 */

#include "rit128x96x4.h"
#include "graphicObjects.h"

// Run Method
void vuGraphicObjectsTestTask(void *pvParameters) {
	pgoButton b;
	pgoButton b1;

	pgoButton b2;
	pgoButton b3;

	goInit();

	b = goNewButton(10, 10, 10, 10, goButtonUp);
	b1 = goNewButton(10, 10, 30, 10, goButtonUp);

	b2 = goNewButton(10, 10, 10, 30, goButtonUp);
	b3 = goNewButton(10, 10, 30, 30, goButtonUp);

	goDrawButtons ();

	goStartListener();

	goDeleteButton(b);
	goDeleteButton(b1);
	goDeleteButton(b2);
	goDeleteButton(b3);
}
