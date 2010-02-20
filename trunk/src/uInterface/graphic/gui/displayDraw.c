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
#include "hw_types.h"
#include "setup.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"

#include "displayDraw.h"
#include "dislpayBasics.h"
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

#if DEBUG_GRAPHIC
	printf("vDrawElementsOnDisplay: %x\n", xDisplayRoot.displayEntities);
#endif

	if (xDisplayRoot.displayEntities == true) {

#if DEBUG_GRAPHIC
		printf("vDrawElementsOnDisplay: Draw Elements offset %d\n",
				elementOffset);
#endif

		toDraw = xDisplayRoot.entities;
		for (i = 0; i < elementOffset && toDraw != NULL && toDraw->next != NULL; i++) {
			toDraw = toDraw->next;

#if DEBUG_GRAPHIC
			printf("vDrawElementsOnDisplay: next Element\n");
#endif
		}


#if DEBUG_GRAPHIC
		printf("vDrawElementsOnDisplay: root = %x, todraw = %x\n",
				(unsigned int) xDisplayRoot.entities, (unsigned int) toDraw);
#endif

		for (i = 0; toDraw != NULL && i < DISPLAY_LINES_PER_VIEW; i++) {
			xGetLabelWidget(toDraw, i);
			if (toDraw->labelWidget != NULL) {
				WidgetAdd((tWidget*) &xParentWidget,
						(tWidget*) toDraw->labelWidget);
			}

			xGetValueWidget(toDraw, i);
			if (toDraw->valueWidget != NULL) {
				WidgetAdd((tWidget*) &xParentWidget,
						(tWidget*) toDraw->valueWidget);
			}

#if DEBUG_GRAPHIC
			printf(
					"vDrawElementsOnDisplay: new id=%s, label=%s, value=%d, strValue=%s\n",
					toDraw->id, toDraw->label, toDraw->value, toDraw->strValue);
#endif
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

	WidgetPaint((tWidget*) &xParentWidget);
}

tWidget* xGetLabelWidget(basicDisplayLine *line, int row) {
	if (line->labelWidget != NULL) {
		vPortFree(line->labelWidget);
	}
	line->labelWidget = pvPortMalloc(sizeof(tCanvasWidget));
	((tCanvasWidget*) line->labelWidget)->pFont = DISPLAY_LABEL_FONT;
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
	case SSI_INDEX_HYPERLINK:
		line->valueWidget = pvPortMalloc(sizeof(tPushButtonWidget));
		((tPushButtonWidget*) line->valueWidget)->pFont = DISPLAY_VALUE_FONT;
		((tPushButtonWidget*) line->valueWidget)->pcText
				= DISPLAY_VALUE_TEXT_HYPERLINK;
		((tPushButtonWidget*) line->valueWidget)->pfnOnClick = vHyperlinkAction;
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
		;
		((tPushButtonWidget*) line->valueWidget)->sBase.sPosition.sYMax = (row
				* (DISPLAY_LINE_HEIGHT + DISPLAY_LINE_MARGIN))
				+ (DISPLAY_TOP_OFFSET) + DISPLAY_LINE_HEIGHT - 1;
		break;
	default:

#if DEBUG_GRAPHIC
		printf("xGetValueWidget: Not Implemented for %d\n", line->type);
#endif

		break;
	}
	return line->labelWidget;
}
