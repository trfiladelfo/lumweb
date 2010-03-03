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

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "drivers/kitronix320x240x16_ssd2119_8bit.h"

int elementOffset = 0;

tWidget* xGetLabelWidget(void*line, int row);
tWidget* xGetValueWidget(void*line, int row);

void vDrawElementsOnDisplay(void) {

	int i;
	basicDisplayLine *toDraw;

	vTaskSuspendAll();
	{

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

				if (toDraw->type->onDisplay != NULL) {
					toDraw->type->onDisplay(toDraw, i);
				} else {
					toDraw->valueWidget = NULL;
				}

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
	xTaskResumeAll();
}

tWidget* xGetLabelWidget(void *akt, int row) {

	basicDisplayLine *line = (basicDisplayLine*) akt;
	if (line->labelWidget != NULL) {
		vPortFree(line->labelWidget);
	}
	line->labelWidget = pvPortMalloc(sizeof(tCanvasWidget));

	if (line->type == xTagList[TAG_INDEX_GROUP]) {
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

