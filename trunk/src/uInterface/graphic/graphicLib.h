/*
 * graphicLib.h
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#ifndef GRAPHICLIB_H_
#define GRAPHICLIB_H_

#include "hw_types.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/pushbutton.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "kitronix320x240x16_ssd2119_8bit.h"

/* Context Structure */
tContext g_sContext;

/* Container for all the Widgets */
tCanvasWidget * xParentContainer;

/* Pointer to the Root Object (= first inserted Object) */
tWidget * xRootObject;

/* Pointer to the last inserted Object */
tWidget * xLastInsertedObject;

tPushButtonWidget *addButton( // Insert new Graphic Button
		int left,
		int top,
		int width,
		int height,
		char* label,
		tBoolean autorepeat,
		void(*callback)(tWidget *pWidget)
	);

tCanvasWidget *addLabel( // Insert new Label
		int left,
		int top,
		int width,
		int height,
		char* text
	);

tSliderWidget *addSlider( // Insert new Slider
		int left,
		int top,
		int width,
		int height,
		char* label,
		long value, void(*callback)(tWidget *pWidget, long value)
	);

tCheckBoxWidget *addCheckbox(
		int left,
		int top,
		int width,
		int height,
		unsigned long bSelected,
		void (*pfnOnChange)(tWidget *pWidget, unsigned long bSelected)
	);

void vDrawPanel(void);
void vDestroyPanel(void);
void vCleanDisplay(void);
void vInitPanel(void);

void vShowBootText(char* textToShow);

#endif /* GRAPHICLIB_H_ */
