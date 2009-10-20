/*
 * graphicButtons.c
 *
 *  Created on: 07.10.2009
 *      Author: anzinger
 */

#include "graphicObjects.h"

#include "rit128x96x4.h"


/*
 * Prototypes
 */

/**
 * Draws a simple Button with Labelimage
 *
 * returns a structure from the Button
 */
pgoButton goNewButton(int height, int width, int left, int top,
		const char unsigned *value, const unsigned char *border, void(*selectAction)(void*), void* pvParam) {

	// Allocate RAM for the Button
	//struct goButton *btn = (struct goButton *) pvPortMalloc(sizeof (goButton));
	pgoButton btn = (pgoButton) pvPortMalloc(sizeof(struct goButton));

	btn->height = height;
	btn->width = width;
	btn->left = left;
	btn->top = top;
	btn->value = value;
	btn->border = border;
	btn->pvParam = pvParam;
	btn->selectAction = selectAction;

	goInsertButton(btn);

	return btn;
}


/*
 * Draws a Button to the Screen
 */
void goDrawButton(pgoButton btn) {
	if (btn->border == NULL) {
		btn->border = pucBorderNormal;
	}
	goDrawBorder(btn->height, btn->width, btn->left, btn->top, btn->border);

	RIT128x96x4ImageDraw(btn->value, btn->left, btn->top, btn->width,
			btn->height);
}

void goDrawButtons(void) {
	pgoButton akt = buttonListRoot;
	while (akt != NULL) {

		goDrawButton(akt);

		akt = akt->next;
	}
}

/*
 * Frees the allocated Memory
 */
void goDeleteButton(pgoButton btn) {
	pgoButton toDelete;
	toDelete = btn;
	btn = btn->next;
	vPortFree(toDelete);
}

/*
 * Returns the next Button, null if there isn't one
 */
pgoButton goGetNextButton(pgoButton btn) {
	return btn->next;
}

/*
 * Returns the Last Button, null if List ist empty
 */
pgoButton goGetLastButton(void) {
	return buttonListLast;
}

/*
 * Returns the Button before, null if empty or first
 */
pgoButton goGetPrevButton(pgoButton btn) {
	pgoButton akt = buttonListRoot;
	while (akt->next != NULL && akt->next != btn) {
		akt = akt->next;
	}
	return akt;
}

/*
 * Returns the First Button, null if empty
 */
pgoButton goGetFirstButton(void) {
	return buttonListRoot;
}

/*
 * Inserts the Button into the List
 */
void goInsertButton(pgoButton btn) {
	if (buttonListRoot == NULL) {
		buttonListRoot = btn;
	} else {
		buttonListLast->next = btn;
	}
	buttonListLast = btn;
}
