/*
 * DefaultTags.c
 *
 *  Created on: 02.03.2010
 *      Author: root
 */

#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/checkbox.h"
#include "grlib/pushbutton.h"

#include "graphic/gui/touchActions.h"


void vDummyOnLoadPtr(char* param, int len, void* this) {
	;
}
tWidget* vDummyOnDisplayPtr(void* this, int row) {

	basicDisplayLine* line = (basicDisplayLine*) this;

	if (line->valueWidget != NULL) {
		vPortFree(line->valueWidget);
	}
	switch (line->type->tagindex) {
	case TAG_INDEX_CHECKBOXINPUTFIELD:
		line->valueWidget = pvPortMalloc(sizeof(tCheckBoxWidget));
		((tCheckBoxWidget*) line->valueWidget)->ulFillColor
				= DISPLAY_VALUE_CHECKBOX_BACKGROUND_COLOR;
		((tCheckBoxWidget*) line->valueWidget)->pcText = NULL;
		((tCheckBoxWidget*) line->valueWidget)->pfnOnChange = vCheckboxAction;
		((tCheckBoxWidget*) line->valueWidget)->pucImage = NULL;
		((tCheckBoxWidget*) line->valueWidget)->ulOutlineColor
				= DISPLAY_VALUE_CHECKBOX_OUTLINE_COLOR;
		((tCheckBoxWidget*) line->valueWidget)->ulTextColor
				= DISPLAY_VALUE_COLOR;
		((tCheckBoxWidget*) line->valueWidget)->usBoxSize = DISPLAY_LINE_HEIGHT;
		((tCheckBoxWidget*) line->valueWidget)->usStyle
				= DISPLAY_VALUE_STYLE_BOOLEAN;

		if (line->value != 0) {
			((tCheckBoxWidget*) line->valueWidget)->usStyle
					|= CB_STYLE_SELECTED;
		}
		((tCheckBoxWidget*) line->valueWidget)->sBase.lSize
				= sizeof(tCheckBoxWidget);
		((tCheckBoxWidget*) line->valueWidget)->sBase.pChild = NULL;
		((tCheckBoxWidget*) line->valueWidget)->sBase.pDisplay = DISPLAY_DRIVER;
		((tCheckBoxWidget*) line->valueWidget)->sBase.pNext = NULL;
		((tCheckBoxWidget*) line->valueWidget)->sBase.pParent = NULL;
		((tCheckBoxWidget*) line->valueWidget)->sBase.pfnMsgProc
				= CheckBoxMsgProc;
		((tCheckBoxWidget*) line->valueWidget)->sBase.sPosition.sXMin
				= DISPLAY_VALUE_LEFT;
		((tCheckBoxWidget*) line->valueWidget)->sBase.sPosition.sYMin = (row
				* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
				+ (DISPLAY_TOP_OFFSET);
		((tCheckBoxWidget*) line->valueWidget)->sBase.sPosition.sXMax
				= DISPLAY_VALUE_LEFT + DISPLAY_VALUE_WIDTH - 1;
		;
		((tCheckBoxWidget*) line->valueWidget)->sBase.sPosition.sYMax = (row
				* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
				+ (DISPLAY_TOP_OFFSET) + DISPLAY_LINE_HEIGHT - 1;
		break;
	case TAG_INDEX_INTEGERINPUTFIELD:
	case TAG_INDEX_FLOATINPUTFIELD:
	case TAG_INDEX_TIMEINPUTFIELD:
	case TAG_INDEX_HYPERLINK:
		line->valueWidget = pvPortMalloc(sizeof(tPushButtonWidget));
		((tPushButtonWidget*) line->valueWidget)->pFont = DISPLAY_VALUE_FONT;
		((tPushButtonWidget*) line->valueWidget)->pucImage = NULL;
		((tPushButtonWidget*) line->valueWidget)->pucPressImage = NULL;
		((tPushButtonWidget*) line->valueWidget)->ulAutoRepeatCount = 0;
		((tPushButtonWidget*) line->valueWidget)->ulFillColor
				= DISPLAY_VALUE_BACKGROUND_COLOR;
		((tPushButtonWidget*) line->valueWidget)->ulOutlineColor
				= DISPLAY_VALUE_OUTLINE_COLOR;
		((tPushButtonWidget*) line->valueWidget)->ulPressFillColor
				= DISPLAY_VALUE_PUSH_COLOR
		;
		((tPushButtonWidget*) line->valueWidget)->ulStyle
				= DISPLAY_VALUE_STYLE_HYPERLINK;
		((tPushButtonWidget*) line->valueWidget)->ulTextColor
				= DISPLAY_VALUE_COLOR;
		((tPushButtonWidget*) line->valueWidget)->usAutoRepeatDelay = 0;
		((tPushButtonWidget*) line->valueWidget)->usAutoRepeatRate = 0;
		((tPushButtonWidget*) line->valueWidget)->sBase.lSize
				= sizeof(tPushButtonWidget);
		((tPushButtonWidget*) line->valueWidget)->sBase.pChild = NULL;
		((tPushButtonWidget*) line->valueWidget)->sBase.pDisplay
				= DISPLAY_DRIVER;
		((tPushButtonWidget*) line->valueWidget)->sBase.pNext = NULL;
		((tPushButtonWidget*) line->valueWidget)->sBase.pParent = NULL;
		((tPushButtonWidget*) line->valueWidget)->sBase.pfnMsgProc
				= RectangularButtonMsgProc;
		((tPushButtonWidget*) line->valueWidget)->sBase.sPosition.sXMin
				= DISPLAY_VALUE_LEFT;
		((tPushButtonWidget*) line->valueWidget)->sBase.sPosition.sYMin = (row
				* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
				+ (DISPLAY_TOP_OFFSET);
		((tPushButtonWidget*) line->valueWidget)->sBase.sPosition.sXMax
				= DISPLAY_VALUE_LEFT + DISPLAY_VALUE_WIDTH - 1;
		((tPushButtonWidget*) line->valueWidget)->sBase.sPosition.sYMax = (row
				* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
				+ (DISPLAY_TOP_OFFSET) + DISPLAY_LINE_HEIGHT - 1;

		if (line->type->tagindex == TAG_INDEX_HYPERLINK) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vHyperlinkAction;
			((tPushButtonWidget*) line->valueWidget)->pcText
					= DISPLAY_VALUE_TEXT_HYPERLINK;
		} else if (line->type->tagindex == TAG_INDEX_INTEGERINPUTFIELD) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vOpenEditorAction;
			((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;
		} else if (line->type->tagindex == TAG_INDEX_FLOATINPUTFIELD) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vOpenEditorAction;
			((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;
		} else if (line->type->tagindex == TAG_INDEX_TIMEINPUTFIELD) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vOpenEditorAction;
			((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;
		}
		break;
	default:

		break;
	}

}
void vDummyOnEditValuePtr(void* this) {
	;
}
void vDummyOnDestroyPtr(void* this) {
	;
}

char* vDummyStrFormatter (void* this) {

	basicDisplayLine* line = (basicDisplayLine*) this;

	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%d", line->value);
	return line->strValue;
}

