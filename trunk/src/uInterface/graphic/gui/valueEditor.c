/*
 * valueEditor.c
 *
 *  Created on: 20.02.2010
 *      Author: root
 */

#include "FreeRTOS.h"
#include "valueEditor.h"

#include "taglib/taglib.h"

#include "graphic/gui/displayBasics.h"
#include "graphic/gui/editorStyle.h"

#include "grlib/grlib.h"
#include "grlib/pushbutton.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"

tPushButtonWidget xIncrease1;
tPushButtonWidget xDecrease1;
tPushButtonWidget xIncrease2;
tPushButtonWidget xDecrease2;
tPushButtonWidget xCancel;
tPushButtonWidget xSubmit;

basicDisplayLine *aktElement;

int iNumChangeButton = 1;

tCanvasWidget xValueWidget;
tCanvasWidget xLabel;
tCanvasWidget xEditorWidget;

char* valueBuffer[2][4];

void vInitializeEditorWidget(void);
void vInitializeIncreaseButton1(void);
void vInitializeIncreaseButton2(void);
void vInitializeDecreaseButton1(void);
void vInitializeDecreaseButton2(void);
void vInitializeSubmitButton(void);
void vExitEditor(tWidget *pWidget);
void vInitializeLabel(void);
void vInitializeValueWidget(void);

void vIncreaseValue(tWidget *pWidget);
void vDecreaseValue(tWidget *pWidget);

void vOpenEditor(basicDisplayLine* akt) {

	aktElement = akt;

	if (akt->type->tagindex != TAG_INDEX_TIMEINPUTFIELD) {
		iNumChangeButton = 1;
	} else {
		iNumChangeButton = 2;
	}

	vInitializeEditorWidget();
	vInitializeIncreaseButton1();
	vInitializeIncreaseButton2();
	vInitializeDecreaseButton1();
	vInitializeDecreaseButton2();
	vInitializeSubmitButton();
	vInitializeValueWidget();
	vInitializeLabel();

	if (akt->label != NULL) {
		xLabel.pcText = akt->label;
	}

	if (akt->strValue != NULL) {
		xValueWidget.pcText = akt->strValue;
	} else {
		xValueWidget.pcText = "undefined";
	}

	if (iNumChangeButton == 2) {
		WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xIncrease1);
		WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xDecrease1);
	} else {
		WidgetRemove((tWidget*) &xIncrease1);
		WidgetRemove((tWidget*) &xDecrease1);
	}

	WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xValueWidget);

	WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xDecrease2);
	WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xIncrease2);
	WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xLabel);

	WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xSubmit);
	//WidgetAdd((tWidget*) &xEditorWidget, (tWidget*) &xCancel);

	WidgetRemove((tWidget*) &xParentWidget);
	WidgetAdd(WIDGET_ROOT, (tWidget*) &xEditorWidget);
	WidgetPaint((tWidget*)&xEditorWidget);
}

/**
 * Initialize the ParentWidget (=root for Drawing) with the Definitions of displayStyle.h
 */
void vInitializeEditorWidget(void) {
	xEditorWidget.pFont = NULL;
	xEditorWidget.pcText = NULL;
	xEditorWidget.pucImage = NULL;
	xEditorWidget.sBase.lSize = sizeof(tPushButtonWidget);
	xEditorWidget.sBase.pChild = NULL;
	xEditorWidget.sBase.pDisplay = DISPLAY_DRIVER;
	xEditorWidget.sBase.pNext = NULL;
	xEditorWidget.sBase.pParent = NULL;
	xEditorWidget.sBase.pfnMsgProc = CanvasMsgProc;
	xEditorWidget.sBase.sPosition.sXMax = GrContextDpyWidthGet(&g_sContext);
	xEditorWidget.sBase.sPosition.sXMin = 0;
	xEditorWidget.sBase.sPosition.sYMax = GrContextDpyHeightGet(&g_sContext);
	xEditorWidget.sBase.sPosition.sYMin = 0;
	xEditorWidget.ulFillColor = ClrBlack;
	xEditorWidget.ulOutlineColor = ClrBlack;
	xEditorWidget.ulStyle = CANVAS_STYLE_FILL;
	xEditorWidget.ulTextColor = ClrBlack;
}

void vInitializeIncreaseButton1(void) {
	xIncrease1.pFont = EDITOR_INC_1_BUTTON_FONT;
	xIncrease1.pcText = EDITOR_INC_1_BUTTON_TEXT;
	xIncrease1.pfnOnClick = EDITOR_INC_1_BUTTON_ACTION;
	xIncrease1.pucImage = NULL;
	xIncrease1.pucPressImage = NULL;
	xIncrease1.sBase.lSize = sizeof(tPushButtonWidget);
	xIncrease1.sBase.pChild = NULL;
	xIncrease1.sBase.pDisplay = DISPLAY_DRIVER;
	xIncrease1.sBase.pNext = NULL;
	xIncrease1.sBase.pParent = NULL;
	xIncrease1.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xIncrease1.sBase.sPosition.sXMax = EDITOR_INC_1_BUTTON_LEFT
			+ EDITOR_INC_1_BUTTON_WIDTH - 1;
	xIncrease1.sBase.sPosition.sXMin = EDITOR_INC_1_BUTTON_LEFT;
	xIncrease1.sBase.sPosition.sYMax = EDITOR_INC_1_BUTTON_TOP
			+ EDITOR_INC_1_BUTTON_HEIGHT - 1;
	xIncrease1.sBase.sPosition.sYMin = EDITOR_INC_1_BUTTON_TOP;
	xIncrease1.ulAutoRepeatCount = 1;
	xIncrease1.ulFillColor = EDITOR_INC_1_BUTTON_BACKGROUND_COLOR;
	xIncrease1.ulOutlineColor = EDITOR_INC_1_BUTTON_OUTLINE_COLOR;
	xIncrease1.ulPressFillColor = EDITOR_INC_1_BUTTON_PUSH_COLOR;
	xIncrease1.ulStyle = EDITOR_INC_1_BUTTON_STYLE | PB_STYLE_AUTO_REPEAT;
	xIncrease1.ulTextColor = EDITOR_INC_1_BUTTON_COLOR;
	xIncrease1.usAutoRepeatDelay = 100;
	xIncrease1.usAutoRepeatRate = 20;
}

void vInitializeIncreaseButton2(void) {
	xIncrease2.pFont = EDITOR_INC_2_BUTTON_FONT;
	xIncrease2.pcText = EDITOR_INC_2_BUTTON_TEXT;
	xIncrease2.pfnOnClick = EDITOR_INC_2_BUTTON_ACTION;
	xIncrease2.pucImage = NULL;
	xIncrease2.pucPressImage = NULL;
	xIncrease2.sBase.lSize = sizeof(tPushButtonWidget);
	xIncrease2.sBase.pChild = NULL;
	xIncrease2.sBase.pDisplay = DISPLAY_DRIVER;
	xIncrease2.sBase.pNext = NULL;
	xIncrease2.sBase.pParent = NULL;
	xIncrease2.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xIncrease2.sBase.sPosition.sXMax = EDITOR_INC_2_BUTTON_LEFT
			+ EDITOR_INC_2_BUTTON_WIDTH - 1;
	xIncrease2.sBase.sPosition.sXMin = EDITOR_INC_2_BUTTON_LEFT;
	xIncrease2.sBase.sPosition.sYMax = EDITOR_INC_2_BUTTON_TOP
			+ EDITOR_INC_2_BUTTON_HEIGHT - 1;
	xIncrease2.sBase.sPosition.sYMin = EDITOR_INC_2_BUTTON_TOP;
	xIncrease2.ulAutoRepeatCount = 1;
	xIncrease2.ulFillColor = EDITOR_INC_2_BUTTON_BACKGROUND_COLOR;
	xIncrease2.ulOutlineColor = EDITOR_INC_2_BUTTON_OUTLINE_COLOR;
	xIncrease2.ulPressFillColor = EDITOR_INC_2_BUTTON_PUSH_COLOR;
	xIncrease2.ulStyle = EDITOR_INC_2_BUTTON_STYLE | PB_STYLE_AUTO_REPEAT;
	xIncrease2.ulTextColor = EDITOR_INC_2_BUTTON_COLOR;
	xIncrease2.usAutoRepeatDelay = 100;
	xIncrease2.usAutoRepeatRate = 20;
}

void vInitializeDecreaseButton1(void) {
	xDecrease1.pFont = EDITOR_DEC_1_BUTTON_FONT;
	xDecrease1.pcText = EDITOR_DEC_1_BUTTON_TEXT;
	xDecrease1.pfnOnClick = EDITOR_DEC_1_BUTTON_ACTION;
	xDecrease1.pucImage = NULL;
	xDecrease1.pucPressImage = NULL;
	xDecrease1.sBase.lSize = sizeof(tPushButtonWidget);
	xDecrease1.sBase.pChild = NULL;
	xDecrease1.sBase.pDisplay = DISPLAY_DRIVER;
	xDecrease1.sBase.pNext = NULL;
	xDecrease1.sBase.pParent = NULL;
	xDecrease1.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xDecrease1.sBase.sPosition.sXMax = EDITOR_DEC_1_BUTTON_LEFT
			+ EDITOR_DEC_1_BUTTON_WIDTH - 1;
	xDecrease1.sBase.sPosition.sXMin = EDITOR_DEC_1_BUTTON_LEFT;
	xDecrease1.sBase.sPosition.sYMax = EDITOR_DEC_1_BUTTON_TOP
			+ EDITOR_DEC_1_BUTTON_HEIGHT - 1;
	xDecrease1.sBase.sPosition.sYMin = EDITOR_DEC_1_BUTTON_TOP;
	xDecrease1.ulAutoRepeatCount = 1;
	xDecrease1.ulFillColor = EDITOR_DEC_1_BUTTON_BACKGROUND_COLOR;
	xDecrease1.ulOutlineColor = EDITOR_DEC_1_BUTTON_OUTLINE_COLOR;
	xDecrease1.ulPressFillColor = EDITOR_DEC_1_BUTTON_PUSH_COLOR;
	xDecrease1.ulStyle = EDITOR_DEC_1_BUTTON_STYLE | PB_STYLE_AUTO_REPEAT;
	xDecrease1.ulTextColor = EDITOR_DEC_1_BUTTON_COLOR;
	xDecrease1.usAutoRepeatDelay = 100;
	xDecrease1.usAutoRepeatRate = 20;
}

void vInitializeDecreaseButton2(void) {
	xDecrease2.pFont = EDITOR_DEC_2_BUTTON_FONT;
	xDecrease2.pcText = EDITOR_DEC_2_BUTTON_TEXT;
	xDecrease2.pfnOnClick = EDITOR_DEC_2_BUTTON_ACTION;
	xDecrease2.pucImage = NULL;
	xDecrease2.pucPressImage = NULL;
	xDecrease2.sBase.lSize = sizeof(tPushButtonWidget);
	xDecrease2.sBase.pChild = NULL;
	xDecrease2.sBase.pDisplay = DISPLAY_DRIVER;
	xDecrease2.sBase.pNext = NULL;
	xDecrease2.sBase.pParent = NULL;
	xDecrease2.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xDecrease2.sBase.sPosition.sXMax = EDITOR_DEC_2_BUTTON_LEFT
			+ EDITOR_DEC_2_BUTTON_WIDTH - 1;
	xDecrease2.sBase.sPosition.sXMin = EDITOR_DEC_2_BUTTON_LEFT;
	xDecrease2.sBase.sPosition.sYMax = EDITOR_DEC_2_BUTTON_TOP
			+ EDITOR_DEC_2_BUTTON_HEIGHT - 1;
	xDecrease2.sBase.sPosition.sYMin = EDITOR_DEC_2_BUTTON_TOP;
	xDecrease2.ulAutoRepeatCount = 1;
	xDecrease2.ulFillColor = EDITOR_DEC_2_BUTTON_BACKGROUND_COLOR;
	xDecrease2.ulOutlineColor = EDITOR_DEC_2_BUTTON_OUTLINE_COLOR;
	xDecrease2.ulPressFillColor = EDITOR_DEC_2_BUTTON_PUSH_COLOR;
	xDecrease2.ulStyle = EDITOR_DEC_2_BUTTON_STYLE | PB_STYLE_AUTO_REPEAT;
	xDecrease2.ulTextColor = EDITOR_DEC_2_BUTTON_COLOR;
	xDecrease2.usAutoRepeatDelay = 100;
	xDecrease2.usAutoRepeatRate = 20;
}

void vInitializeSubmitButton(void) {
	xSubmit.pFont = EDITOR_SUBMIT_BUTTON_FONT;
	xSubmit.pcText = EDITOR_SUBMIT_BUTTON_TEXT;
	xSubmit.pfnOnClick = EDITOR_SUBMIT_BUTTON_ACTION;
	xSubmit.pucImage = NULL;
	xSubmit.pucPressImage = NULL;
	xSubmit.sBase.lSize = sizeof(tPushButtonWidget);
	xSubmit.sBase.pChild = NULL;
	xSubmit.sBase.pDisplay = DISPLAY_DRIVER;
	xSubmit.sBase.pNext = NULL;
	xSubmit.sBase.pParent = NULL;
	xSubmit.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xSubmit.sBase.sPosition.sXMax = EDITOR_SUBMIT_BUTTON_LEFT
			+ EDITOR_SUBMIT_BUTTON_WIDTH - 1;
	xSubmit.sBase.sPosition.sXMin = EDITOR_SUBMIT_BUTTON_LEFT;
	xSubmit.sBase.sPosition.sYMax = EDITOR_SUBMIT_BUTTON_TOP
			+ EDITOR_SUBMIT_BUTTON_HEIGHT - 1;
	xSubmit.sBase.sPosition.sYMin = EDITOR_SUBMIT_BUTTON_TOP;
	xSubmit.ulAutoRepeatCount = 0;
	xSubmit.ulFillColor = EDITOR_SUBMIT_BUTTON_BACKGROUND_COLOR;
	xSubmit.ulOutlineColor = EDITOR_SUBMIT_BUTTON_OUTLINE_COLOR;
	xSubmit.ulPressFillColor = EDITOR_SUBMIT_BUTTON_PUSH_COLOR;
	xSubmit.ulStyle = EDITOR_SUBMIT_BUTTON_STYLE;
	xSubmit.ulTextColor = EDITOR_SUBMIT_BUTTON_COLOR;
	xSubmit.usAutoRepeatDelay = 0;
	xSubmit.usAutoRepeatRate = 0;
}

/**
 * Initialize the Title with the Definitions of displayStyle.h
 */
void vInitializeLabel(void) {
	xLabel.pFont = DISPLAY_TITLE_FONT;
	xLabel.pcText = DISPLAY_TITLE_TEXT;
	xLabel.pucImage = NULL;
	xLabel.sBase.lSize = sizeof(tPushButtonWidget);
	xLabel.sBase.pChild = NULL;
	xLabel.sBase.pDisplay = DISPLAY_DRIVER;
	xLabel.sBase.pNext = NULL;
	xLabel.sBase.pParent = NULL;
	xLabel.sBase.pfnMsgProc = CanvasMsgProc;
	xLabel.sBase.sPosition.sXMax = DISPLAY_TITLE_LEFT + DISPLAY_TITLE_WIDTH - 1;
	xLabel.sBase.sPosition.sXMin = DISPLAY_TITLE_LEFT;
	xLabel.sBase.sPosition.sYMax = DISPLAY_TITLE_TOP + DISPLAY_TITLE_HEIGHT - 1;
	xLabel.sBase.sPosition.sYMin = DISPLAY_TITLE_TOP;
	xLabel.ulFillColor = DISPLAY_TITLE_BACKGROUND_COLOR;
	xLabel.ulOutlineColor = DISPLAY_TITLE_OUTLINE_COLOR;
	xLabel.ulStyle = DISPLAY_TITLE_STYLE;
	xLabel.ulTextColor = DISPLAY_TITLE_COLOR;
}

/**
 * Initialize the Title with the Definitions of displayStyle.h
 */
void vInitializeValueWidget(void) {
	xValueWidget.pFont = EDITOR_VALUE_FONT;
	xValueWidget.pucImage = NULL;
	xValueWidget.sBase.lSize = sizeof(tPushButtonWidget);
	xValueWidget.sBase.pChild = NULL;
	xValueWidget.sBase.pDisplay = DISPLAY_DRIVER;
	xValueWidget.sBase.pNext = NULL;
	xValueWidget.sBase.pParent = NULL;
	xValueWidget.sBase.pfnMsgProc = CanvasMsgProc;
	xValueWidget.sBase.sPosition.sXMax = EDITOR_VALUE_LEFT + EDITOR_VALUE_WIDTH
			- 1;
	xValueWidget.sBase.sPosition.sXMin = EDITOR_VALUE_LEFT;
	xValueWidget.sBase.sPosition.sYMax = EDITOR_VALUE_TOP + EDITOR_VALUE_HEIGHT
			- 1;
	xValueWidget.sBase.sPosition.sYMin = EDITOR_VALUE_TOP;
	xValueWidget.ulFillColor = EDITOR_VALUE_BACKGROUND_COLOR;
	xValueWidget.ulOutlineColor = EDITOR_VALUE_OUTLINE_COLOR;
	xValueWidget.ulStyle = EDITOR_VALUE_STYLE;
	xValueWidget.ulTextColor = EDITOR_VALUE_COLOR;
}

void vExitEditor(tWidget *pWidget) {
	WidgetRemove((tWidget*) &xEditorWidget);
	WidgetAdd(WIDGET_ROOT, (tWidget*) &xParentWidget);
	WidgetPaint((tWidget*) &xParentWidget);
}

void vIncreaseValue(tWidget *pWidget) {
	switch (aktElement->type->tagindex) {

	case TAG_INDEX_INTEGERINPUTFIELD:
		aktElement->value += aktElement->increment;

		if (aktElement->min != aktElement->max && aktElement->value
				> aktElement->max) {
			aktElement->value = aktElement->min + (aktElement->value
					- aktElement->max) - 1;
		}
		xValueWidget.pcText = (const char*) aktElement->type->strFormatter(aktElement);
		break;
	case TAG_INDEX_FLOATINPUTFIELD:
		aktElement->value += aktElement->increment;
		if (aktElement->min != aktElement->max && aktElement->value
				> aktElement->max) {
			aktElement->value = aktElement->min + (aktElement->value
					- aktElement->max) - 1;
		}
		xValueWidget.pcText = (const char*) aktElement->type->strFormatter(aktElement);
		break;
	case TAG_INDEX_TIMEINPUTFIELD:
		if (pWidget == (tWidget *) &xIncrease1) {
			aktElement->value += 60;
		} else if (pWidget == (tWidget *) &xIncrease2) {
			aktElement->value++;
		}

		aktElement->value = aktElement->value % 1440;
		xValueWidget.pcText = (const char*) aktElement->type->strFormatter(aktElement);
		break;
	default:
		break;
	}

	WidgetPaint((tWidget*) &xValueWidget);
}

void vDecreaseValue(tWidget *pWidget) {
	switch (aktElement->type->tagindex) {

	case TAG_INDEX_INTEGERINPUTFIELD:
		aktElement->value -= aktElement->increment;
		if (aktElement->min != aktElement->max && aktElement->value
				< aktElement->min) {
			aktElement->value = aktElement->max + (aktElement->value
					- aktElement->min) + 1;
		}

		xValueWidget.pcText = (const char*) aktElement->type->strFormatter(aktElement);
		break;
	case TAG_INDEX_FLOATINPUTFIELD:
		aktElement->value -= aktElement->increment;
		if (aktElement->min != aktElement->max && aktElement->value
				< aktElement->min) {
			aktElement->value = aktElement->max + (aktElement->value
					- aktElement->min) + 1;
		}
		xValueWidget.pcText = (const char*) aktElement->type->strFormatter(aktElement);
		break;
	case TAG_INDEX_TIMEINPUTFIELD:
		if (pWidget == (tWidget *) &xDecrease1) {
			aktElement->value -= 60;
		} else if (pWidget == (tWidget *) &xDecrease2) {
			aktElement->value--;
		}

		if (aktElement->value < 0) {
			aktElement->value = 1440 + (aktElement->value);
		}
		xValueWidget.pcText =(const char*)  aktElement->type->strFormatter(aktElement);
		break;
	default:
		break;
	}
	WidgetPaint((tWidget*) &xValueWidget);
}
