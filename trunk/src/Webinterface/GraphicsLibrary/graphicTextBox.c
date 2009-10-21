/*
 * graphicBextBoxs.c
 *
 *  Created on: 20.10.2009
 *      Author: anzinger
 */

#include "string.h"
#include "graphicObjects.h"
#include "rit128x96x4.h"

/**
 * Draws a simple BextBox with Labelimage
 *
 * returns a structure from the BextBox
 */
pgoTextBox goNewTextBox(int size, int left, int top, int *value) {

	// Allocate RAM for the TextBox
	pgoTextBox txt = (pgoTextBox) pvPortMalloc(sizeof(struct goTextBox));

	txt->size = size;
	txt->left = left;
	txt->top = top;
	txt->value = value;

	goInsertTextBox(txt);

	return txt;
}

/*
 * Draws a TextBox to the Screen
 */
void goDrawTextBox(pgoTextBox txt) {
	unsigned char buffer[32];
	unsigned int height, width;

	goDrawBorder((CHAR_HEIGHT + 2), ((txt->size * (CHAR_WIDTH + 1)) + 2),
			txt->left, txt->top, pucBorderDeactivated);

	sprintf(buffer, "%d", *txt->value);
	//sprintf(buffer, "%d", xTaskGetTickCount());
	RIT128x96x4StringDraw(buffer, txt->left + 2, txt->top + 1, 0xF);
}

void goDrawTextBoxes(void) {
	pgoTextBox akt = textBoxListRoot;
	while (akt != NULL) {

		goDrawTextBox(akt);
		akt = akt->next;
	}
}

/*
 * Frees the allocated Memory
 */
void goDeleteTextBox(pgoTextBox txt) {
	pgoTextBox toDelete;
	toDelete = txt;
	txt = txt->next;
	vPortFree(toDelete);
}

/*
 * Returns the next TextBox, null if there isn't one
 */
pgoTextBox goGetNextTextBox(pgoTextBox txt) {
	return txt->next;
}

/*
 * Returns the Last TextBox, null if List ist empty
 */
pgoTextBox goGetLastTextBox(void) {
	return textBoxListLast;
}

/*
 * Returns the TextBox before, null if empty or first
 */
pgoTextBox goGetPrevTextBox(pgoTextBox txt) {
	pgoTextBox akt = textBoxListRoot;
	while (akt->next != NULL && akt->next != txt) {
		akt = akt->next;
	}
	return akt;
}

/*
 * Returns the First TextBox, null if empty
 */
pgoTextBox goGetFirstTextBox(void) {
	return textBoxListRoot;
}

/*
 * Inserts the TextBox into the List
 */
void goInsertTextBox(pgoTextBox txt) {
	if (textBoxListRoot == NULL) {
		textBoxListRoot = txt;
	} else {
		textBoxListLast->next = txt;
	}
	textBoxListLast = txt;
}

// Testroutine
void vTextBoxIncrement(void* pvParam) {
	pgoTextBox txt = (pgoTextBox) pvParam;
	*(txt->value) += 1;
}
// Testroutine
void vTextBoxDecrement(void* pvParam) {
	pgoTextBox txt = (pgoTextBox) pvParam;
	*(txt->value) -= 1;
}
