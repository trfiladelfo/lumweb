/**
 * \addtogroup Graphic
 * @{
 *
 * \file graphicLib.c
 * \author Anziner, Hahn
 * \brief
 *
*/

#include "FreeRTOS.h"

#include "setup.h"

#include "hw_types.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"
#include "grlib/slider.h"
#include "kitronix320x240x16_ssd2119_8bit.h"

#include "graphic/graphicLib.h"

/* Header Rectangle */
tRectangle sRect;

int contextInitialized = 0;

void vInitPanel(void) {
	//
	// Initialize the graphics context.
	//
	if (!contextInitialized) {
#ifdef DEBUG_GRAPHIC
		printf("Initialize Graphic Context ...\n");
#endif
		GrContextInit(&g_sContext, &g_sKitronix320x240x16_SSD2119);
		contextInitialized = 1;

	} else {
#ifdef DEBUG_GRAPHIC
		printf("Clear Display\n");
#endif
		vCleanDisplay();

	}
#ifdef DEBUG_GRAPHIC
	printf("Adding ROOT Widget ...\n");
#endif
	WidgetAdd(WIDGET_ROOT, (tWidget*) xParentContainer);

}

/**
 * Adds a Button to the Panel
 */
tPushButtonWidget *addButton(int left, int top, int width, int height,
		char* label, tBoolean autorepeat, void(*callback)(tWidget *pWidget)) {
	tPushButtonWidget * aktButton = (tPushButtonWidget*) pvPortMalloc(
			sizeof(tPushButtonWidget));

	aktButton->pFont = &g_sFontCm14b;
	aktButton->pcText = label;
	aktButton->pfnOnClick = callback;
	aktButton->pucImage = 0;
	aktButton->pucPressImage = 0;
	aktButton->sBase.lSize = sizeof(tPushButtonWidget);
	aktButton->sBase.pParent = (tWidget*) xParentContainer;
	aktButton->sBase.pChild = 0;
	aktButton->sBase.pDisplay = &g_sKitronix320x240x16_SSD2119;
	aktButton->sBase.pNext = 0;
	aktButton->sBase.pfnMsgProc = RectangularButtonMsgProc;
	aktButton->sBase.sPosition.sXMin = left;
	aktButton->sBase.sPosition.sYMin = top;
	aktButton->sBase.sPosition.sXMax = left + width - 1;
	aktButton->sBase.sPosition.sYMax = top + height - 1;
	aktButton->ulFillColor = ClrMidnightBlue;
	aktButton->ulOutlineColor = ClrGray;
	aktButton->ulPressFillColor = ClrGray;
	aktButton->ulStyle = PB_STYLE_FILL | PB_STYLE_OUTLINE | PB_STYLE_TEXT;
	aktButton->ulTextColor = ClrWhite;

	if (autorepeat) {
		aktButton->ulStyle |= PB_STYLE_AUTO_REPEAT;
		aktButton->usAutoRepeatDelay = 100; // change for Autorepeat
		aktButton->usAutoRepeatRate = 20;// change for Autorepeat
		aktButton->ulAutoRepeatCount = 1; // change for Autorepeat
	} else {
		aktButton->usAutoRepeatDelay = 0; // change for Autorepeat
		aktButton->usAutoRepeatRate = 0;// change for Autorepeat
		aktButton->ulAutoRepeatCount = 0; // change for Autorepeat
	}
	if (xLastInsertedObject) {
		xLastInsertedObject->pNext = (tWidget*) aktButton;
	}

	if (xRootObject == 0) {
		xRootObject = (tWidget *) aktButton;
	}
	xLastInsertedObject = (tWidget *) aktButton;

	return aktButton;
}

tCanvasWidget *addLabel(int left, int top, int width, int height, char* text) {
	tCanvasWidget * aktLabel = (tCanvasWidget*) pvPortMalloc(
			sizeof(tCanvasWidget));

	aktLabel->pFont = &g_sFontCm14b;
	aktLabel->pcText = text;
	aktLabel->pucImage = 0;
	aktLabel->sBase.lSize = sizeof(tCanvasWidget);
	aktLabel->sBase.pParent = (tWidget*) xParentContainer;
	aktLabel->sBase.pChild = 0;
	aktLabel->sBase.pDisplay = &g_sKitronix320x240x16_SSD2119;
	aktLabel->sBase.pNext = 0;
	aktLabel->sBase.pfnMsgProc = CanvasMsgProc;
	aktLabel->sBase.sPosition.sXMin = left;
	aktLabel->sBase.sPosition.sYMin = top;
	aktLabel->sBase.sPosition.sXMax = left + width - 1;
	aktLabel->sBase.sPosition.sYMax = top + height - 1;
	aktLabel->ulFillColor = ClrBlack;
	aktLabel->ulOutlineColor = ClrGray;
	aktLabel->ulStyle = CANVAS_STYLE_TEXT | CANVAS_STYLE_FILL;
	aktLabel->ulTextColor = ClrWhite;

	if (xLastInsertedObject) {
		xLastInsertedObject->pNext = (tWidget*) aktLabel;
	}

	if (!xRootObject) {
		xRootObject = (tWidget *) aktLabel;
	}
	xLastInsertedObject = (tWidget *) aktLabel;

	return aktLabel;
}

tCheckBoxWidget *addCheckbox(int left, int top, int width, int height,
		unsigned long bSelected, void(*pfnOnChange)(tWidget *pWidget,
				unsigned long bSelected)) {
	tCheckBoxWidget * aktCheckbox = (tCheckBoxWidget*) pvPortMalloc(
			sizeof(tCheckBoxWidget));

	aktCheckbox->pFont = &g_sFontCm14b;
	aktCheckbox->pcText = "";
	aktCheckbox->pucImage = 0;
	aktCheckbox->sBase.lSize = sizeof(tCanvasWidget);
	aktCheckbox->sBase.pParent = (tWidget*) xParentContainer;
	aktCheckbox->sBase.pChild = 0;
	aktCheckbox->sBase.pDisplay = &g_sKitronix320x240x16_SSD2119;
	aktCheckbox->sBase.pNext = 0;
	aktCheckbox->sBase.pfnMsgProc = CheckBoxMsgProc;
	aktCheckbox->sBase.sPosition.sXMin = left;
	aktCheckbox->sBase.sPosition.sYMin = top;
	aktCheckbox->sBase.sPosition.sXMax = left + width - 1;
	aktCheckbox->sBase.sPosition.sYMax = top + height - 1;
	aktCheckbox->ulFillColor = ClrBlack;
	aktCheckbox->ulOutlineColor = ClrWhite;
	aktCheckbox->pfnOnChange = pfnOnChange;
	aktCheckbox->usStyle = CB_STYLE_FILL;
	aktCheckbox->usBoxSize = 30;

	if (bSelected) {
		aktCheckbox->usStyle |= CB_STYLE_SELECTED;
	}

	if (xLastInsertedObject) {
		xLastInsertedObject->pNext = (tWidget*) aktCheckbox;
	}

	if (!xRootObject) {
		xRootObject = (tWidget *) aktCheckbox;
	}
	xLastInsertedObject = (tWidget *) aktCheckbox;

	return aktCheckbox;
}

tSliderWidget *addSlider(int left, int top, int width, int height, char* label,
		long value, void(*callback)(tWidget *pWidget, long value)) {
	tSliderWidget * aktSlider = (tSliderWidget*) pvPortMalloc(
			sizeof(tSliderWidget));

	aktSlider->pFont = &g_sFontCm14b;
	aktSlider->pcText = label;

	aktSlider->lValue = value;
	aktSlider->lMax = 0;
	aktSlider->lMin = 100;

	aktSlider->pucImage = 0;

	aktSlider->sBase.lSize = sizeof(tSliderWidget);
	aktSlider->sBase.pParent = (tWidget*) xParentContainer;
	aktSlider->sBase.pChild = 0;
	aktSlider->sBase.pDisplay = &g_sKitronix320x240x16_SSD2119;
	aktSlider->sBase.pNext = 0;
	aktSlider->sBase.pfnMsgProc = SliderMsgProc;
	aktSlider->sBase.sPosition.sXMin = left;
	aktSlider->sBase.sPosition.sYMin = top;
	aktSlider->sBase.sPosition.sXMax = left + width - 1;
	aktSlider->sBase.sPosition.sYMax = top + height - 1;

	aktSlider->pfnOnChange = callback;

	aktSlider->pucBackgroundImage = 0;
	aktSlider->sPos = 0;

	aktSlider->ulTextColor = ClrWhite;
	aktSlider->ulBackgroundTextColor = ClrWhite;
	aktSlider->ulFillColor = ClrMidnightBlue;
	aktSlider->ulBackgroundFillColor = ClrGreen;
	aktSlider->ulOutlineColor = ClrGray;

	aktSlider->ulStyle = SL_STYLE_BACKG_FILL | SL_STYLE_FILL | SL_STYLE_OUTLINE
			| SL_STYLE_TEXT | SL_STYLE_BACKG_TEXT;

	if (xLastInsertedObject) {
		xLastInsertedObject->pNext = (tWidget*) aktSlider;
	}

	if (!xRootObject) {
		xRootObject = (tWidget *) aktSlider;
	}
	xLastInsertedObject = (tWidget *) aktSlider;

	return aktSlider;
}

void vDrawPanel(void) {
	//
	// Issue the initial paint request to the widgets.
	//
	if (xParentContainer) {
#ifdef DEBUG_GRAPHIC
		printf("output (0x%X)\n", xRootObject);
#endif
		xParentContainer->sBase.pChild = xRootObject;
		WidgetPaint((tWidget*)xParentContainer);
	}
}

/**
 * Frees the RAM for any Widget (recursive)
 */
void vDestroyWidget(tWidget* toDestroy) {
	if (toDestroy != NULL) {
		vDestroyWidget(toDestroy->pChild);
		vDestroyWidget(toDestroy->pNext);
		WidgetRemove(toDestroy);
		if (toDestroy != NULL) {
			vPortFree(toDestroy);
			toDestroy = NULL;
		}
#ifdef DEBUG_GRAPHIC
		printf("toDestroy 0x%x\n", toDestroy);
#endif

	}
}

/**
 * Cleans the Display and clears all the child objects
 */
void vCleanDisplay() {
	if (xParentContainer) {
		vDestroyPanel();
		WidgetPaint((tWidget*)xParentContainer);
	}
}

/**
 * Free all the RAM from the actual Panel
 */
void vDestroyPanel(void) {
	if (xParentContainer != 0) {
		vDestroyWidget(xParentContainer->sBase.pChild);
		vDestroyWidget(xParentContainer->sBase.pNext);
	}
}

void vShowBootText(char* textToShow) {

	/* Header Rectangle */
	tRectangle sRect;

	if (g_sContext.pDisplay == 0) {
#ifdef DEBUG_GRAPHIC
		printf("Initialize Graphic Context ...\n");
#endif
		GrContextInit(&g_sContext, &g_sKitronix320x240x16_SSD2119);
	}

	//
	// Fill the top 24 rows of the screen with blue to create the banner.
	//
	sRect.sXMin = 5;
	sRect.sYMin = 5;
	sRect.sXMax = GrContextDpyWidthGet(&g_sContext) - 5;
	sRect.sYMax = GrContextDpyHeightGet(&g_sContext) - 5;

	GrContextForegroundSet(&g_sContext, ClrRed);
	GrRectFill(&g_sContext, &sRect);

	//
	// Put a white box around the banner.
	//
	GrContextForegroundSet(&g_sContext, ClrWhite);
	GrRectDraw(&g_sContext, &sRect);

	//
	// Put the application name in the middle of the banner.
	//
	GrContextFontSet(&g_sContext, &g_sFontCm14);
	GrStringDrawCentered(&g_sContext, textToShow, -1,
			GrContextDpyWidthGet(&g_sContext) / 2, GrContextDpyHeightGet(&g_sContext) / 2, 0);
}

