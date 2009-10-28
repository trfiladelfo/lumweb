/*
 * graphicBextBoxs.c
 *
 *  Created on: 20.10.2009
 *      Author: anzinger
 */

#include "string.h"
#include "graphicObjects.h"
#include "graphicTextbox.h"
#include "rit128x96x4.h"

#include "DebugUART/debugUART.h"
#include "ComTask/comTask.h"

/**
 * Draws a simple BextBox with Labelimage
 *
 * returns a structure from the BextBox
 */
pgoTextBox goNewTextBox(int size, int left, int top, char *commTaskLink)
{

	// Allocate RAM for the TextBox
	pgoTextBox txt = (pgoTextBox) pvPortMalloc(sizeof(struct goTextBox));

	txt->size = size;
	txt->left = left;
	txt->top = top;
	txt->commTaskLink = commTaskLink;

	txt->value = iTextBoxGetValue(commTaskLink);

	goInsertTextBox(txt);

	if (textBoxListSelected == NULL)
	{
		textBoxListSelected = txt;
	}

	return txt;
}

/*
 * Draws a TextBox to the Screen
 */
void goDrawTextBox(pgoTextBox txt)
{
	unsigned char buffer[32];
	unsigned int height, width;
	const unsigned char *border = pucTextboxNormal;
	if (textBoxListSelected == txt)
	{
		border = pucTextboxSelected;
	}

	goDrawBorder((CHAR_HEIGHT + 2), ((txt->size * (CHAR_WIDTH + 1)) + 2),
			txt->left, txt->top, border);

	sprintf(buffer, "%d", txt->value);
	//sprintf(buffer, "%d", xTaskGetTickCount());
	RIT128x96x4StringDraw(buffer, txt->left + 2, txt->top + 1, 0xF);
}

void goDrawTextBoxes(void)
{
	pgoTextBox akt = textBoxListRoot;
	while (akt != NULL)
	{

		goDrawTextBox(akt);
		akt = akt->next;
	}
}

/*
 * Frees the allocated Memory
 */
void goDeleteTextBox(pgoTextBox txt)
{
	pgoTextBox toDelete;
	toDelete = txt;
	txt = txt->next;
	vPortFree(toDelete);
}

/*
 * Returns the next TextBox, null if there isn't one
 */
pgoTextBox goGetNextTextBox(pgoTextBox txt)
{
	if (txt->next == NULL)
	{
		return textBoxListRoot;
	}
	return txt->next;
}

/*
 * Returns the Last TextBox, null if List ist empty
 */
pgoTextBox goGetLastTextBox(void)
{
	return textBoxListLast;
}

/*
 * Returns the TextBox before, null if empty or first
 */
pgoTextBox goGetPrevTextBox(pgoTextBox txt)
{
	pgoTextBox akt = textBoxListRoot;
	while (akt->next != NULL && akt->next != txt)
	{
		akt = akt->next;
	}
	return akt;
}

/*
 * Returns the First TextBox, null if empty
 */
pgoTextBox goGetFirstTextBox(void)
{
	return textBoxListRoot;
}

/*
 * Inserts the TextBox into the List
 */
void goInsertTextBox(pgoTextBox txt)
{
	if (textBoxListRoot == NULL)
	{
		textBoxListRoot = txt;
	}
	else
	{
		textBoxListLast->next = txt;
	}
	textBoxListLast = txt;
}

// Testroutine
void vTextBoxIncrement(void* pvParam)
{
	(textBoxListSelected->value) += 1;
	goDrawTextBox(textBoxListSelected);
	vSendDebugUART(textBoxListSelected->commTaskLink);
}
// Testroutine
void vTextBoxDecrement(void* pvParam)
{
	(textBoxListSelected->value) -= 1;
	goDrawTextBox(textBoxListSelected);
	vSendDebugUART(textBoxListSelected->commTaskLink);
}

int iTextBoxGetValue(char *value)
{
	xCOMMessage xMessage;
	xGraphCommunication xGraphicMessage;
	int i;

	xMessage.item = value;
	xMessage.cmd = GET;
	xMessage.from = GRAPHIC;

	xQueueSend(xCOMQueue, &xMessage, (portTickType) 0);

	//vTaskSuspend(xGraphicObjectsTaskHandler);

	if (xQueueReceive(xGraphCommunicationQueue, &xGraphicMessage, (portTickType) 100))
	{
		return xGraphicMessage.value;
	}
	return 0;
}

void vTextBoxSetValues(void)
{
	xCOMMessage xMessage;
	pgoTextBox akt = textBoxListRoot;
	while (akt != NULL)
	{
		xMessage.item = akt->commTaskLink;
		xMessage.value = akt->value;
		xMessage.cmd = SET;
		xMessage.from = GRAPHIC;

		xQueueSend(xCOMQueue, &xMessage, (portTickType) 0);

		akt = akt->next;
	}
}
