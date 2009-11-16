/*
 * graphicBextBoxs.c
 *
 *  Created on: 20.10.2009
 *      Author: anzinger
 */

#include <string.h>
#include "graphicObjects.h"
#include "graphicTextBox.h"
#include "renderGraphics.h"
#include "rit128x96x4.h"

#include "communication/comTask.h"
#include "../queueConfig.h"
#include "../taskConfig.h"

tBoolean overflowTop, overflowBottom;
tBoolean offsetChanged = false;

xComMessage xMessage;

/**
 * Draws a simple TextBox with Labelimage
 *
 * returns a structure from the TextBox
 */
pgoTextBox goNewTextBox(int size, int left, int top, char *commTaskLink) {
	// Allocate RAM for the TextBox
	pgoTextBox txt = (pgoTextBox) pvPortMalloc(sizeof(struct goTextBox));

	// Write the Parameters into the structure
	txt->size = size;
	txt->left = left;
	txt->top = top;
	txt->commTaskLink = commTaskLink;

	// get the Value for the Box
	printf("TEXTBOX: hole wert fuer Textbox (%s)\n", commTaskLink);
	txt->value = iTextBoxGetValue(commTaskLink);

	// insert the Box into the Display Queue
	printf("TEXTBOX: Fuege Textbox ein\n");
	goInsertTextBox(txt);

	// If no Box is Selected (= if it is the first) set the Box activated
	if (textBoxListSelected == NULL) {
		textBoxListSelected = txt;
	}

	// Return the actual Instance
	return txt;
}

/*
 * Draws one TextBox to the Screen
 */
void goDrawTextBox(pgoTextBox txt) {
	unsigned char buffer[16];
	unsigned int height, width;
	char color = 0x0A;
	const unsigned char *border = pucTextboxNormal;

	if (textBoxListSelected->top + offset < 0) {
		offset += LINE_HEIGHT;
		offsetChanged = true;
	} else if (textBoxListSelected->top + offset > (VISIBLE_HEIGHT
			- OBJECT_HEIGHT)) {
		offset -= LINE_HEIGHT;
		offsetChanged = true;
	}

	if (textBoxListSelected == textBoxListRoot) {
		offset = 0;
	}

	if (txt->top + offset < 0) {
		overflowTop = true;
	} else if (txt->top + offset > (VISIBLE_HEIGHT - OBJECT_HEIGHT)) {
		overflowBottom = true;
	} else {
		if (textBoxListSelected == txt) {
			border = pucTextboxSelected;
			color = 0x0F;
		}

		if (offsetChanged == true) {
			RIT128x96x4StringDraw("             ", txt->left, txt->top + 1
					+ offset, 0x0);
		}
		RIT128x96x4StringDraw(txt->commTaskLink, txt->left, txt->top + 1
				+ offset, 0xF);
		//goDrawBorder(OBJECT_HEIGHT, ((txt->size * (CHAR_WIDTH + 1)) + 2),
		//		txt->left + 75, txt->top + offset, border);

		sprintf(buffer, "%d", txt->value);
		RIT128x96x4StringDraw("          ", txt->left + 77, txt->top + 1
				+ offset, 0x0);
		RIT128x96x4StringDraw(buffer, txt->left + 77, txt->top + 1 + offset,
				color);
	}
}

/**
 * Draw all Textboxes to the Screen
 */
void goDrawTextBoxes(void) {
	pgoTextBox akt = textBoxListRoot;

	overflowTop = false;
	overflowBottom = false;
	offsetChanged = false;

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
 * Returns the next TextBox, null if there isn't one, rootTextBox if it is after the last
 */
pgoTextBox goGetNextTextBox(pgoTextBox txt) {
	if (txt->next == NULL) {
		return textBoxListRoot;
	}
	return txt->next;
}

/*
 * Returns the Last TextBox, null if List is empty
 */
pgoTextBox goGetLastTextBox(void) {
	return textBoxListLast;
}

/*
 * Returns the TextBox before, null if empty, last if it is befor the first
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

/*
 * Increments the Textbox Value
 */
void vTextBoxIncrement(void* pvParam) {
	(textBoxListSelected->value) += 1;
	goDrawTextBox(textBoxListSelected);
}

/*
 * Decrements the Textbox Value
 */
void vTextBoxDecrement(void* pvParam) {
	(textBoxListSelected->value) -= 1;
	goDrawTextBox(textBoxListSelected);
}

/*
 * Function to Load the Value form the Communication Task
 */
int iTextBoxGetValue(char *nameOfValue) {
	xMessage.item = nameOfValue;
	xMessage.cmd = GET;
	xMessage.from = xGraphQueue;
	xMessage.dataSouce = DATA;
	xMessage.freeItem = pdFALSE;
	xMessage.taskToResume = xGraphTaskHandle;

	xQueueSend(xComQueue, &xMessage, (portTickType) 0);

	vTaskSuspend(xMessage.taskToResume);

	if (xQueueReceive(xMessage.from, &xMessage, (portTickType) 100)) {
		if (xMessage.errorDesc != NULL) {
			printf("TEXTBOX: %s", xMessage.errorDesc);
		} else {
			return xMessage.value;
		}
	} else {
		printf("TEXTBOX: %s - use default wert\n", xMessage.item);
		return -999;
	}
}

/*
 * Writes the Value to the Communication Task
 */
void vTextBoxSetValues(void) {
	pgoTextBox akt = textBoxListRoot;
	while (akt != NULL) {
		xMessage.item = akt->commTaskLink;
		xMessage.value = akt->value;
		xMessage.cmd = SET;
		xMessage.dataSouce = DATA;
		xMessage.freeItem = pdFALSE;
		xMessage.from = xGraphQueue;
		xMessage.taskToResume = xGraphTaskHandle;

		printf("TEXTBOX: Speichere Wert (%s = %d)\n", xMessage.item, xMessage.value);
		xQueueSend(xComQueue, &xMessage, (portTickType) 0);

		vTaskSuspend(xGraphTaskHandle);

		akt = akt->next;
	}
}

/*
 * This Function refreshes all the Values from the Textboxes
 */
void vTextBoxGetValues(void) {
	pgoTextBox akt = textBoxListRoot;
	while (akt != NULL) {
		akt->value = iTextBoxGetValue(akt->commTaskLink);
	}
}
