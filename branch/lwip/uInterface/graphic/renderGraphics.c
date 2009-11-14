/*
 * renderGraphics.c
 *
 *  Created on: 01.11.2009
 *      Author: root
 */
#include "renderGraphics.h"

#include "rit128x96x4.h"
#include "graphicObjects.h"
#include "graphicTextbox.h"

void render (char** toDisplay, int nrOfElements) {
	int i;
	int textboxLeft, labelLeft, top = 10;

	for (i = 0; i < nrOfElements; i++) {
		textboxLeft = (VISIBLE_WIDTH - 5 - (5 * (CHAR_WIDTH + 1)));
		labelLeft = textboxLeft - 3 - ((CHAR_WIDTH + 1) * sizeof (toDisplay[i]));

		goNewTextBox(5, 5, top, toDisplay[i]);

		top += LINE_HEIGHT;
	}
}

