/**
 * \addtogroup Graphic
 * @{
 *
 * \file displayDraw.c
 * \author Anziner, Hahn
 * \brief Implementation of the Draw Methods
 *
 */

#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "hw_types.h"
#include "setup.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"

#include "displayDraw.h"
#include "displayStyle.h"
#include "touchActions.h"

#include "ethernet/httpd/cgi/io.h"

#include "drivers/kitronix320x240x16_ssd2119_8bit.h"

int elementOffset = 0;

tWidget* xGetLabelWidget(basicDisplayLine *line, int row);
tWidget* xGetValueWidget(basicDisplayLine *line, int row);

void vDrawElementsOnDisplay(void) {

	int i;
	basicDisplayLine *toDraw;

	if (xDisplayRoot.menue == true) {
		WidgetAdd((tWidget*) &xParentWidget, (tWidget*) &xMenuButton);
	} else if (xMenuButton.sBase.pParent != NULL) {
		WidgetRemove((tWidget*) &xMenuButton);
	}

	if (xDisplayRoot.save == true) {
		WidgetAdd((tWidget*) &xParentWidget, (tWidget*) &xSaveButton);
	} else {
		WidgetRemove((tWidget*) &xSaveButton);
	}

	WidgetAdd((tWidget*) &xParentWidget, (tWidget*) &xTitle);

	if (xDisplayRoot.displayEntities == true) {

#if DEBUG_GRAPHIC
		printf("vDrawElementsOnDisplay: Draw Elements offset %d\n",
				elementOffset);
#endif

		toDraw = xDisplayRoot.entities;

		if (elementOffset > 0) {
			for (i = 0; i < elementOffset && toDraw != NULL && toDraw->next
					!= NULL; i++) {
				toDraw = toDraw->next;
			}
		}

		for (i = 0; toDraw != NULL && i < DISPLAY_LINES_PER_VIEW; i++) {
			xGetLabelWidget(toDraw, i);
			if (toDraw->labelWidget != NULL) {
#if DEBUG_GRAPHIC
				printf("vDrawElementsOnDisplay: addLabel\n");
#endif
				WidgetAdd((tWidget*) &xParentWidget,
						(tWidget*) toDraw->labelWidget);
			}

			xGetValueWidget(toDraw, i);
			if (toDraw->valueWidget != NULL) {
#if DEBUG_GRAPHIC
				printf("vDrawElementsOnDisplay: addValue\n");
#endif
				WidgetAdd((tWidget*) &xParentWidget,
						(tWidget*) toDraw->valueWidget);
			}
			toDraw = toDraw->next;
		}

		if (elementOffset > 0) {
			WidgetAdd((tWidget*) &xParentWidget, (tWidget*) &xUpButton);
			xDisplayRoot.up = true;
		} else {
			WidgetRemove((tWidget*) &xUpButton);
			xDisplayRoot.up = false;
		}

		if (toDraw != NULL) {
			xDisplayRoot.down = true;
			WidgetAdd((tWidget*) &xParentWidget, (tWidget*) &xDownButton);
		} else {
			xDisplayRoot.down = false;
			WidgetRemove((tWidget*) &xDownButton);
		}
	}
#if DEBUG_GRAPHIC
	printf("vDrawElementsOnDisplay: draw Elements ... ");
#endif
	WidgetPaint((tWidget*) &xParentWidget);
#if DEBUG_GRAPHIC
	printf("finished\n");
#endif
}

tWidget* xGetLabelWidget(basicDisplayLine *line, int row) {
	if (line->labelWidget != NULL) {
		vPortFree(line->labelWidget);
	}
	line->labelWidget = pvPortMalloc(sizeof(tCanvasWidget));

	if (line->type == SSI_INDEX_GROUP) {
		((tCanvasWidget*) line->labelWidget)->pFont = DISPLAY_LABEL_GROUP_FONT;
	} else {
		((tCanvasWidget*) line->labelWidget)->pFont = DISPLAY_LABEL_FONT;
	}
	((tCanvasWidget*) line->labelWidget)->pcText = line->label;
	((tCanvasWidget*) line->labelWidget)->pucImage = NULL;
	((tCanvasWidget*) line->labelWidget)->ulFillColor
			= DISPLAY_LABEL_BACKGROUND_COLOR;
	((tCanvasWidget*) line->labelWidget)->ulOutlineColor = ClrBlack;
	((tCanvasWidget*) line->labelWidget)->ulStyle = DISPLAY_LABEL_STYLE;
	((tCanvasWidget*) line->labelWidget)->ulTextColor = DISPLAY_LABEL_COLOR;

	((tCanvasWidget*) line->labelWidget)->sBase.lSize = sizeof(tCanvasWidget);
	((tCanvasWidget*) line->labelWidget)->sBase.pChild = NULL;
	((tCanvasWidget*) line->labelWidget)->sBase.pDisplay = DISPLAY_DRIVER;
	((tCanvasWidget*) line->labelWidget)->sBase.pNext = NULL;
	((tCanvasWidget*) line->labelWidget)->sBase.pParent = NULL;
	((tCanvasWidget*) line->labelWidget)->sBase.pfnMsgProc = CanvasMsgProc;
	((tCanvasWidget*) line->labelWidget)->sBase.sPosition.sXMin
			= DISPLAY_LABEL_LEFT;
	((tCanvasWidget*) line->labelWidget)->sBase.sPosition.sYMin = (row
			* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
			+ (DISPLAY_TOP_OFFSET);
	((tCanvasWidget*) line->labelWidget)->sBase.sPosition.sXMax
			= DISPLAY_LABEL_LEFT + DISPLAY_LABEL_WIDTH - 1;
	((tCanvasWidget*) line->labelWidget)->sBase.sPosition.sYMax = (row
			* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
			+ (DISPLAY_TOP_OFFSET) + DISPLAY_LINE_HEIGHT - 1;

	return line->labelWidget;
}

tWidget* xGetValueWidget(basicDisplayLine *line, int row) {
	if (line->valueWidget != NULL) {
		vPortFree(line->valueWidget);
	}
	switch (line->type) {
	case SSI_INDEX_CHECKBOXINPUTFIELD:
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
	case SSI_INDEX_INTEGERINPUTFIELD:
	case SSI_INDEX_FLOATINPUTFIELD:
	case SSI_INDEX_TIMEINPUTFIELD:
	case SSI_INDEX_HYPERLINK:
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

		if (line->type == SSI_INDEX_HYPERLINK) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vHyperlinkAction;
			((tPushButtonWidget*) line->valueWidget)->pcText
					= DISPLAY_VALUE_TEXT_HYPERLINK;
		} else if (line->type == SSI_INDEX_INTEGERINPUTFIELD) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vIntegerEditorAction;
			((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;
		} else if (line->type == SSI_INDEX_FLOATINPUTFIELD) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vFloatEditorAction;
			((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;
		} else if (line->type == SSI_INDEX_TIMEINPUTFIELD) {
			((tPushButtonWidget*) line->valueWidget)->pfnOnClick
					= vTimeEditorAction;
			((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;
		}
		break;
	default:

#if DEBUG_GRAPHIC
		printf("xGetValueWidget: Not Implemented for %d\n", line->type);
#endif

		break;
	}
	return line->labelWidget;
}
