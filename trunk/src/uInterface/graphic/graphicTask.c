/**
 * \addtogroup Graphic
 * @{
 *
 * \file graphicTask.c
 * \author Anziner, Hahn
 * \brief Implementation of Graphics Task for the LM3S9B96 Board
 *
*/

#include "FreeRTOS.h"
#include "task.h"
#include "setup.h"

#include "graphic/graphicTask.h"
#include "graphic/graphicWebClient.h"
#include "graphic/graphicLib.h"

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

//*****************************************************************************
//
// The sixth panel, which contains a selection of push buttons.
//
//*****************************************************************************

void vGraphicTask(void* pvParameters) {

#ifdef DEBUG_GRAPHIC
	printf("Initialize Graphic Task ...\n");
#endif

	xParentContainer = (tCanvasWidget*) pvPortMalloc(sizeof(tCanvasWidget));

	xParentContainer->sBase.lSize = sizeof(tCanvasWidget);

	xParentContainer->sBase.pParent = 0;
	xParentContainer->sBase.pDisplay = &g_sKitronix320x240x16_SSD2119;
	xParentContainer->sBase.pNext = 0;
	xParentContainer->sBase.pChild = 0;
	xParentContainer->sBase.pfnMsgProc = CanvasMsgProc;
	xParentContainer->sBase.sPosition.sXMin = 0;
	xParentContainer->sBase.sPosition.sYMin = 0;
	xParentContainer->sBase.sPosition.sXMax = GrContextDpyWidthGet(&g_sContext);
	xParentContainer->sBase.sPosition.sYMax
			= GrContextDpyHeightGet(&g_sContext);

	xParentContainer->pFont = 0;
	xParentContainer->pcText = 0;
	xParentContainer->pfnOnPaint = 0;
	xParentContainer->pucImage = 0;
	xParentContainer->ulFillColor = ClrBlack;
	xParentContainer->ulOutlineColor = 0;
	xParentContainer->ulStyle = CANVAS_STYLE_FILL;
	xParentContainer->ulTextColor = 0;

	vShowBootText("booting ...");

	//
	// Loop forever handling widget messages.
	//
#ifdef DEBUG_GRAPHIC
	printf("Waiting for Touch Interrups ...\n");
#endif
	while (1) {
		//
		// Process any messages in the widget message queue.
		//
		WidgetMessageQueueProcess();
		vTaskDelay(1);
	}
}
