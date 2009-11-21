/*
 * graphicButton.h
 *
 *  Created on: 28.10.2009
 *      Author: root
 */

#ifndef GRAPHICBUTTON_H_
#define GRAPHICBUTTON_H_

#include "graphicObjects.h"

struct goButton
{
	int height;
	int width;
	int left;
	int top;
	const char unsigned* value;
	const char unsigned* border;
	struct goButton * next;
	void (*selectAction)(void* pvParam);
};

typedef struct goButton * pgoButton;

//
// Button List Pointer
//
pgoButton buttonListRoot;
pgoButton buttonListLast;
pgoButton buttonSelected;

void goDrawBorder(int height_, int width_, int left, int top,
		unsigned const char * type); // Draws a Border for a Object

pgoButton goNewButton(int height, int width, int left, int top,
		const char unsigned *value, const unsigned char *border,
		void(*selectAction)(void*)); // Creates a new Button

void goDrawButton(pgoButton btn); // Draws the Button

void goDrawButtons(void); // Draws all Buttons

void goDeleteButton(pgoButton btn); // Deletes the Button

pgoButton goGetNextButton(pgoButton btn);

pgoButton goGetLastButton(void);

pgoButton goGetPrevButton(pgoButton btn);

pgoButton goGetFirstButton(void);

void goInsertButton(pgoButton btn);

void goRemoveButton(pgoButton btn);


#endif /* GRAPHICBUTTON_H_ */
