/*
 * graphicWebClient.c
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#include "graphic/graphicWebClient.h"
#include "graphic/graphicLib.h"

void increase(tWidget *pWidget) {
	printf("increase aufgerufen\n");
}

void decrease(tWidget *pWidget) {
	printf("decrease aufgerufen\n");
}


void loadMenu (void) {

	initPanel("Hallo Welt!!!");

	addLabel(5, 35, 160, 30, "Name1");
	addButton(165, 35, 30, 30, "-", decrease);
	addLabel(200, 35, 80, 30, "Wert1");
	addButton(285, 35, 30, 30, "+", increase);

	addLabel(5, 70, 160, 30, "Name2");
	addButton(165, 70, 30, 30, "-", decrease);
	addLabel(200, 70, 80, 30, "Wert2");
	addButton(285, 70, 30, 30, "+", increase);

	addLabel(5, 105, 160, 30, "Name3");
	addButton(165, 105, 30, 30, "-", decrease);
	addLabel(200, 105, 80, 30, "Wert3");
	addButton(285, 105, 30, 30, "+", increase);

	addLabel(5, 140, 160, 30, "Name4");
	addButton(165, 140, 30, 30, "-", decrease);
	addLabel(200, 140, 80, 30, "Wert4");
	addButton(285, 140, 30, 30, "+", increase);

	addLabel(5, 175, 160, 30, "Name5");
	addButton(165, 175, 30, 30, "-", decrease);
	addLabel(200, 175, 80, 30, "Wert5");
	addButton(285, 175, 30, 30, "+", increase);

	addButton(5, 210, 80, 30, "Back", decrease);
	addButton(90, 210, 140, 30, "Speichern", increase);
	addButton(235, 210, 80, 30, "Continue", increase);

	drawPanel();
}


