/*
 * graphicTextbox.h
 *
 *  Created on: 28.10.2009
 *      Author: anzinger
 */

#ifndef GRAPHICTEXTBOX_H_
#define GRAPHICTEXTBOX_H_

#include "graphicObjects.h"

static const unsigned char pucTextboxNormal[] =
{ 0x0A, 0x0A, 0x0A, 0x0A };
static const unsigned char pucTextboxSelected[] =
{ 0x0F, 0x0A, 0x0F, 0x0A };

struct goTextBox
{
	int size;
	int left;
	int top;
	char* commTaskLink;
	int value;
	struct goTextBox * next;
};

typedef struct goTextBox * pgoTextBox;

//
// TextBox List Pointer
//
pgoTextBox textBoxListRoot;
pgoTextBox textBoxListLast;
pgoTextBox textBoxListSelected;

//
// TextBox Functions
//
pgoTextBox goNewTextBox(int size, int left, int top, char* value); // Creates a new TextBox

void goDrawTextBox(pgoTextBox txt); // Draws the TextBox

void goDrawTextBoxes(void); // Draws all TextBoxs

void goDeleteTextBox(pgoTextBox txt); // Deletes the TextBox

pgoTextBox goGetNextTextBox(pgoTextBox txt);

pgoTextBox goGetLastTextBox(void);

pgoTextBox goGetPrevTextBox(pgoTextBox txt);

pgoTextBox goGetFirstTextBox(void);

void goInsertTextBox(pgoTextBox txt);

void goRemoveTextBox(pgoTextBox txt);

void vTextBoxIncrement(void* pvParam);

void vTextBoxDecrement(void* pvParam);

void vTextBoxSetValues(void);

#endif /* GRAPHICTEXTBOX_H_ */
