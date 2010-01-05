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


void initPanel    (char* headerText);
void addButton    (int left, int top, int width, int height, char* label, void (*callback)(tWidget *pWidget));
void addLabel     (int left, int top, int width, int height, char* text);
void addSlider    (int left, int top, int width, int height, char* label, long value, void (*callback)(tWidget *pWidget, long value));
void drawPanel 	  (void);
void destroyPanel (void);

#endif /* GRAPHICLIB_H_ */
