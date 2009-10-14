/*
 * graphicButtons.c
 *
 *  Created on: 07.10.2009
 *      Author: root
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
		const char unsigned *value, void(*selectAction)(void)) {

	// Allocate RAM for the Button
	//struct goButton *btn = (struct goButton *) pvPortMalloc(sizeof (goButton));
	pgoButton btn = (pgoButton) pvPortMalloc(sizeof(struct goButton));

	btn->height = height;
	btn->width = width;
	btn->left = left;
	btn->top = top;
	btn->value = value;
	btn->selectAction = selectAction;

	goInsertButton(btn);

	return btn;
}

void goDrawEmptyButton(int height_, int width_, int left, int top,
		unsigned const char * type) {
	unsigned char * pucImage;
	unsigned char * pucImageDraw;

	int i;
	int height = height_ + 4;
	int width = width_ + 4;

	pucImage = (unsigned char *) pvPortMalloc(height * width);
	pucImageDraw = (unsigned char *) pvPortMalloc(height * width / 2);

	for (i = 0; i < width * height; i++) {
		pucImage[i] = 0xFF;
	}

	for (i = 0; i < width; i++) {
		pucImage[i] = type[0];
		pucImage[(width * (height - 1)) + i] = type[0];
	}

	for (i = 1; i < height - 1; i++) {
		pucImage[i * width] = type[1];
		pucImage[i * width + width - 1] = type[1];
	}

	for (i = 1; i < width - 1; i++) {
		pucImage[width + i] = type[2];
		pucImage[(width * (height - 2)) + i] = type[2];
	}

	for (i = 2; i < height - 2; i++) {
		pucImage[i * width + 1] = type[3];
		pucImage[i * width + width - 2] = type[3];
	}

	for (i = 0; i < width * height / 2; i++) {
		pucImageDraw[i] = (pucImage[2 * i] << 4) | (pucImage[2 * i + 1] & 0x0F);
	}

	RIT128x96x4ImageDraw(pucImageDraw, left - 2, top - 2, width, height);

	vPortFree(pucImage);
	vPortFree(pucImageDraw);

}

/*
 * Draws a Button to the Screen
 */
void goDrawButton(pgoButton btn, unsigned const char* type) {
	if (type == NULL) {
		type = pucBorderNormal;
	}
	goDrawEmptyButton(btn->height, btn->width, btn->left, btn->top, type);

	RIT128x96x4ImageDraw(btn->value, btn->left, btn->top, btn->width,
			btn->height);
}

void goDrawButtons(void) {
	pgoButton akt = buttonListRoot;
	while (akt != NULL) {

		goDrawButton(akt, NULL);

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
