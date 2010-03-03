/*
 * dislpayBasics.h
 *
 *  Created on: 19.02.2010
 *      Author: root
 */

#ifndef DISLPAYBASICS_H_
#define DISLPAYBASICS_H_

#include "FreeRTOS.h"
#include "hw_types.h"

#include "taglib/taglib.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/pushbutton.h"
#include "grlib/canvas.h"

#include "graphic/gui/displayStyle.h"

#include "kitronix320x240x16_ssd2119_8bit.h"

#define DISPLAY_DRIVER	&g_sKitronix320x240x16_SSD2119

/* Context Structure */
tContext g_sContext;

extern tCanvasWidget xParentWidget;
extern tCanvasWidget xTitle;

extern tPushButtonWidget xMenuButton;
extern tPushButtonWidget xSaveButton;
extern tPushButtonWidget xDownButton;
extern tPushButtonWidget xUpButton;

// Basic Entity on the LCD
typedef struct __basicDisplayLine {
	taglib* type;
	int value;
	int max;
	int min;
	int increment;
	char* label;
	char* id;
	char strValue[DISPLAY_VALUE_TEXT_LEN];
	tWidget *labelWidget;
	tWidget *valueWidget;

	struct __basicDisplayLine *next;
} basicDisplayLine;

// Describtion of the BasicDisplayLayout
typedef struct __basicDisplaySruct {
	char* title;
	tBoolean titleIsAllocated;
	tBoolean menue;
	tBoolean save;
	tBoolean down;
	tBoolean up;
	basicDisplayLine *entities;
	tBoolean displayEntities;
} basicDisplay;

extern basicDisplay xDisplayRoot;

void vShowBootText(char*);
void vInitDisplay(void);
void vDeleteDisplayLines(basicDisplayLine *root);
void vClearDisplay(void);
void vSetTitle(char*);
void vLoadMenu(void);
void vShowStatusMessage(char* text);
void vRemoveStatusMessage(tBoolean bRefresh);

void vInitializeParentWidget(void);
void vInitializeMenuButton(void);
void vInitializeSaveButton(void);
void vInitializeUpButton(void);
void vInitializeDownButton(void);
void vInitializeMenuButton(void);
void vInitializeTitle(void);

#endif /* DISLPAYBASICS_H_ */
