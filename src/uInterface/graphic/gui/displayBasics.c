/**
 * \addtogroup Graphic
 * @{
 *
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "hw_types.h"
#include "displayBasics.h"
#include "displayStyle.h"
#include "displayDraw.h"

#include "touchActions.h"

#include "grlib/grlib.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"

#include "graphic/httpc/webClient.h"

#include "drivers/kitronix320x240x16_ssd2119_8bit.h"

#include "configuration/configloader.h"

#include "setup.h"

tBoolean displayInitialized = false;

// Objects
basicDisplay xDisplayRoot;

tCanvasWidget xParentWidget;
tCanvasWidget xTitle;
tCanvasWidget xStatusMessage;
tPushButtonWidget xMenuButton;
tPushButtonWidget xSaveButton;
tPushButtonWidget xDownButton;
tPushButtonWidget xUpButton;

// Prototypes
void vDeleteDisplayLines(basicDisplayLine *root);
void vDeleteDisplayLine(basicDisplayLine *toDelete);
void vClearDisplay(void);

void vInitializeMenuButton(void);
void vInitializeSaveButton(void);
void vInitializeUpButton(void);
void vInitializeDownButton(void);
void vInitializeParentWidget(void);
void vInitializeTitle(void);

// Functions

/**
 * Initializes the Display
 */
void vInitDisplay(void)
{

	//
	// Initialize the graphics context.
	//
	if (g_sContext.pDisplay == NULL)
	{
		GrContextInit(&g_sContext, DISPLAY_DRIVER);
	}

	if (displayInitialized == false)
	{
		if (xDisplayRoot.titleIsAllocated == true)
		{
			vPortFree(xDisplayRoot.title);
			xDisplayRoot.title = NULL;
			xDisplayRoot.titleIsAllocated = false;
		}
		else
		{
			xDisplayRoot.title = NULL;
		}

		xDisplayRoot.entities = NULL;

		xDisplayRoot.down = false;
		xDisplayRoot.up = false;
		xDisplayRoot.menue = true;
		xDisplayRoot.save = false;

		vDeleteDisplayLines(xDisplayRoot.entities);
		xDisplayRoot.displayEntities = false;

		vInitializeParentWidget();
		vInitializeMenuButton();
		vInitializeSaveButton();
		vInitializeUpButton();
		vInitializeDownButton();
		vInitializeMenuButton();
		vInitializeTitle();

		displayInitialized = true;
	}
	vSetTitle(xDisplayRoot.title);
	WidgetAdd(WIDGET_ROOT, (tWidget*) &xParentWidget);
}

/**
 * Deletes all Lines from the Screen
 */
void vDeleteDisplayLines(basicDisplayLine *root)
{
	basicDisplayLine * akt;
	if (xDisplayRoot.displayEntities == true)
	{
		while (root != NULL)
		{
			akt = root;
			if (root->label != NULL)
			{
#if DEBUG_GRAPHIC
				printf("vDeleteDisplayLines: root->name = %s\n", root->label);
#endif
			}
			root = root->next;
			vDeleteDisplayLine(akt);
		}
	}
	xDisplayRoot.displayEntities = false;
	xDisplayRoot.save = false;
	elementOffset = 0;
}

/**
 * Deletes one Line from the Screen and sets the Pointer to the Next one
 */
void vDeleteDisplayLine(basicDisplayLine *toDelete)
{
	if (toDelete != NULL)
	{
		if (toDelete->type->onDestroy != NULL)
		{
			toDelete->type->onDestroy(toDelete);
		}

		if (toDelete->labelWidget != NULL)
		{
			WidgetRemove(toDelete->labelWidget);
			vPortFree(toDelete->labelWidget);
		}

		if (toDelete->valueWidget != NULL)
		{
			WidgetRemove(toDelete->valueWidget);
			vPortFree(toDelete->valueWidget);
		}

		if (toDelete->label != NULL)
		{
			vPortFree(toDelete->label);
		}

		if (toDelete->id != NULL)
		{
			vPortFree(toDelete->id);
		}

		vPortFree(toDelete);
		toDelete = NULL;
	}
}

/**
 * Load the Menue Page form the Web
 */
void vLoadMenu(void)
{
	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");

	vInitDisplay();

#if DEBUG_GRAPHIC
	printf("vLoadMenu: Display deleted\nLoad new Page\n");
#endif

	vLoadWebPage(configLoad, NULL);
	vPortFree(configLoad);
	xDisplayRoot.menue = false;
	vDrawElementsOnDisplay();
}

/**
 * Clear all Elements on the Screen (no Redrawing, use vDrawElementsOnDisplay to redraw)
 */
void vClearDisplay(void)
{
	vDeleteDisplayLines(xDisplayRoot.entities);
}

/**
 * Show the Text for the Bootscreen
 */
void vShowBootText(char* text)
{

	/* Header Rectangle */
	tRectangle sRect;

	if (g_sContext.pDisplay == 0)
	{
		GrContextInit(&g_sContext, DISPLAY_DRIVER);
	}

	//
	// Fill the top 24 rows of the screen with blue to create the banner.
	//
	sRect.sXMin = 0;
	sRect.sYMin = 0;
	sRect.sXMax = GrContextDpyWidthGet(&g_sContext);
	sRect.sYMax = GrContextDpyHeightGet(&g_sContext);

	GrContextForegroundSet(&g_sContext, DISPLAY_BOOT_SCREEN_BACKGROUND_COLOR);
	GrContextBackgroundSet(&g_sContext, DISPLAY_BOOT_SCREEN_BACKGROUND_COLOR);
	GrRectFill(&g_sContext, &sRect);

	//
	// Put a white box around the banner.
	//
	GrRectDraw(&g_sContext, &sRect);

	GrContextForegroundSet(&g_sContext, DISPLAY_BOOT_SCREEN_COLOR);
	//
	// Put the application name in the middle of the banner.
	//
	GrContextFontSet(&g_sContext, DISPLAY_BOOT_SCREEN_FONT);
	GrStringDrawCentered(&g_sContext, text, -1,
			GrContextDpyWidthGet(&g_sContext) / 2, GrContextDpyHeightGet(&g_sContext) / 2, 0);
}

/**
 * Initialize the Menue Button with the Definitions of displayStyle.h
 */
void vInitializeMenuButton(void)
{
	xMenuButton.pFont = DISPLAY_MENU_BUTTON_FONT;
	xMenuButton.pcText = DISPLAY_MENU_BUTTON_TEXT;
	xMenuButton.pfnOnClick = DISPLAY_MENU_BUTTON_ACTION;
	xMenuButton.pucImage = NULL;
	xMenuButton.pucPressImage = NULL;
	xMenuButton.sBase.lSize = sizeof(tPushButtonWidget);
	xMenuButton.sBase.pChild = NULL;
	xMenuButton.sBase.pDisplay = DISPLAY_DRIVER;
	xMenuButton.sBase.pNext = NULL;
	xMenuButton.sBase.pParent = NULL;
	xMenuButton.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xMenuButton.sBase.sPosition.sXMax = DISPLAY_MENU_BUTTON_LEFT
			+ DISPLAY_MENU_BUTTON_WIDTH - 1;
	xMenuButton.sBase.sPosition.sXMin = DISPLAY_MENU_BUTTON_LEFT;
	xMenuButton.sBase.sPosition.sYMax = DISPLAY_MENU_BUTTON_TOP
			+ DISPLAY_MENU_BUTTON_HEIGHT - 1;
	xMenuButton.sBase.sPosition.sYMin = DISPLAY_MENU_BUTTON_TOP;
	xMenuButton.ulAutoRepeatCount = 0;
	xMenuButton.ulFillColor = DISPLAY_MENU_BUTTON_BACKGROUND_COLOR;
	xMenuButton.ulOutlineColor = DISPLAY_MENU_BUTTON_OUTLINE_COLOR;
	xMenuButton.ulPressFillColor = DISPLAY_MENU_BUTTON_PUSH_COLOR;
	xMenuButton.ulStyle = DISPLAY_MENU_BUTTON_STYLE;
	xMenuButton.ulTextColor = DISPLAY_MENU_BUTTON_COLOR;
	xMenuButton.usAutoRepeatDelay = 0;
	xMenuButton.usAutoRepeatRate = 0;
}

/**
 * Initialize the Save Button with the Definitions of displayStyle.h
 */
void vInitializeSaveButton(void)
{
	xSaveButton.pFont = DISPLAY_SAVE_BUTTON_FONT;
	xSaveButton.pcText = DISPLAY_SAVE_BUTTON_TEXT;
	xSaveButton.pfnOnClick = DISPLAY_SAVE_BUTTON_ACTION;
	xSaveButton.pucImage = NULL;
	xSaveButton.pucPressImage = NULL;
	xSaveButton.sBase.lSize = sizeof(tPushButtonWidget);
	xSaveButton.sBase.pChild = NULL;
	xSaveButton.sBase.pDisplay = DISPLAY_DRIVER;
	xSaveButton.sBase.pNext = NULL;
	xSaveButton.sBase.pParent = NULL;
	xSaveButton.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xSaveButton.sBase.sPosition.sXMax = DISPLAY_SAVE_BUTTON_LEFT
			+ DISPLAY_SAVE_BUTTON_WIDTH - 1;
	xSaveButton.sBase.sPosition.sXMin = DISPLAY_SAVE_BUTTON_LEFT;
	xSaveButton.sBase.sPosition.sYMax = DISPLAY_SAVE_BUTTON_TOP
			+ DISPLAY_SAVE_BUTTON_HEIGHT - 1;
	xSaveButton.sBase.sPosition.sYMin = DISPLAY_SAVE_BUTTON_TOP;
	xSaveButton.ulAutoRepeatCount = 0;
	xSaveButton.ulFillColor = DISPLAY_SAVE_BUTTON_BACKGROUND_COLOR;
	xSaveButton.ulOutlineColor = DISPLAY_SAVE_BUTTON_OUTLINE_COLOR;
	xSaveButton.ulPressFillColor = DISPLAY_SAVE_BUTTON_PUSH_COLOR;
	xSaveButton.ulStyle = DISPLAY_SAVE_BUTTON_STYLE;
	xSaveButton.ulTextColor = DISPLAY_SAVE_BUTTON_COLOR;
	xSaveButton.usAutoRepeatDelay = 0;
	xSaveButton.usAutoRepeatRate = 0;
}

/**
 * Initialize the Up Button with the Definitions of displayStyle.h
 */
void vInitializeUpButton(void)
{
	xUpButton.pFont = DISPLAY_UP_BUTTON_FONT;
	xUpButton.pcText = DISPLAY_UP_BUTTON_TEXT;
	xUpButton.pfnOnClick = DISPLAY_UP_BUTTON_ACTION;
	xUpButton.pucImage = NULL;
	xUpButton.pucPressImage = NULL;
	xUpButton.sBase.lSize = sizeof(tPushButtonWidget);
	xUpButton.sBase.pChild = NULL;
	xUpButton.sBase.pDisplay = DISPLAY_DRIVER;
	xUpButton.sBase.pNext = NULL;
	xUpButton.sBase.pParent = NULL;
	xUpButton.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xUpButton.sBase.sPosition.sXMax = DISPLAY_UP_BUTTON_LEFT
			+ DISPLAY_UP_BUTTON_WIDTH - 1;
	xUpButton.sBase.sPosition.sXMin = DISPLAY_UP_BUTTON_LEFT;
	xUpButton.sBase.sPosition.sYMax = DISPLAY_UP_BUTTON_TOP
			+ DISPLAY_UP_BUTTON_HEIGHT - 1;
	xUpButton.sBase.sPosition.sYMin = DISPLAY_UP_BUTTON_TOP;
	xUpButton.ulAutoRepeatCount = 0;
	xUpButton.ulFillColor = DISPLAY_UP_BUTTON_BACKGROUND_COLOR;
	xUpButton.ulOutlineColor = DISPLAY_UP_BUTTON_OUTLINE_COLOR;
	xUpButton.ulPressFillColor = DISPLAY_UP_BUTTON_PUSH_COLOR;
	xUpButton.ulStyle = DISPLAY_UP_BUTTON_STYLE;
	xUpButton.ulTextColor = DISPLAY_UP_BUTTON_COLOR;
	xUpButton.usAutoRepeatDelay = 0;
	xUpButton.usAutoRepeatRate = 0;

	xDisplayRoot.up = false;
}

/**
 * Initialize the Down Button with the Definitions of displayStyle.h
 */
void vInitializeDownButton(void)
{
	xDownButton.pFont = DISPLAY_DOWN_BUTTON_FONT;
	xDownButton.pcText = DISPLAY_DOWN_BUTTON_TEXT;
	xDownButton.pfnOnClick = DISPLAY_DOWN_BUTTON_ACTION;
	xDownButton.pucImage = NULL;
	xDownButton.pucPressImage = NULL;
	xDownButton.sBase.lSize = sizeof(tPushButtonWidget);
	xDownButton.sBase.pChild = NULL;
	xDownButton.sBase.pDisplay = DISPLAY_DRIVER;
	xDownButton.sBase.pNext = NULL;
	xDownButton.sBase.pParent = NULL;
	xDownButton.sBase.pfnMsgProc = RectangularButtonMsgProc;
	xDownButton.sBase.sPosition.sXMax = DISPLAY_DOWN_BUTTON_LEFT
			+ DISPLAY_DOWN_BUTTON_WIDTH - 1;
	xDownButton.sBase.sPosition.sXMin = DISPLAY_DOWN_BUTTON_LEFT;
	xDownButton.sBase.sPosition.sYMax = DISPLAY_DOWN_BUTTON_TOP
			+ DISPLAY_DOWN_BUTTON_HEIGHT - 1;
	xDownButton.sBase.sPosition.sYMin = DISPLAY_DOWN_BUTTON_TOP;
	xDownButton.ulAutoRepeatCount = 0;
	xDownButton.ulFillColor = DISPLAY_DOWN_BUTTON_BACKGROUND_COLOR;
	xDownButton.ulOutlineColor = DISPLAY_DOWN_BUTTON_OUTLINE_COLOR;
	xDownButton.ulPressFillColor = DISPLAY_DOWN_BUTTON_PUSH_COLOR;
	xDownButton.ulStyle = DISPLAY_DOWN_BUTTON_STYLE;
	xDownButton.ulTextColor = DISPLAY_DOWN_BUTTON_COLOR;
	xDownButton.usAutoRepeatDelay = 0;
	xDownButton.usAutoRepeatRate = 0;

	xDisplayRoot.down = false;
}

/**
 * Initialize the ParentWidget (=root for Drawing) with the Definitions of displayStyle.h
 */
void vInitializeParentWidget(void)
{
	xParentWidget.pFont = NULL;
	xParentWidget.pcText = NULL;
	xParentWidget.pucImage = NULL;
	xParentWidget.sBase.lSize = sizeof(tPushButtonWidget);
	xParentWidget.sBase.pChild = NULL;
	xParentWidget.sBase.pDisplay = DISPLAY_DRIVER;
	xParentWidget.sBase.pNext = NULL;
	xParentWidget.sBase.pParent = NULL;
	xParentWidget.sBase.pfnMsgProc = CanvasMsgProc;
	xParentWidget.sBase.sPosition.sXMax = GrContextDpyWidthGet(&g_sContext);
	xParentWidget.sBase.sPosition.sXMin = 0;
	xParentWidget.sBase.sPosition.sYMax = GrContextDpyHeightGet(&g_sContext);
	xParentWidget.sBase.sPosition.sYMin = 0;
	xParentWidget.ulFillColor = ClrBlack;
	xParentWidget.ulOutlineColor = ClrBlack;
	xParentWidget.ulStyle = CANVAS_STYLE_FILL;
	xParentWidget.ulTextColor = ClrBlack;
}

/**
 * Initialize the Title with the Definitions of displayStyle.h
 */
void vInitializeTitle(void)
{
	xTitle.pFont = DISPLAY_TITLE_FONT;
	xTitle.pcText = DISPLAY_TITLE_TEXT;
	xTitle.pucImage = NULL;
	xTitle.sBase.lSize = sizeof(tPushButtonWidget);
	xTitle.sBase.pChild = NULL;
	xTitle.sBase.pDisplay = DISPLAY_DRIVER;
	xTitle.sBase.pNext = NULL;
	xTitle.sBase.pParent = NULL;
	xTitle.sBase.pfnMsgProc = CanvasMsgProc;
	xTitle.sBase.sPosition.sXMax = DISPLAY_TITLE_LEFT + DISPLAY_TITLE_WIDTH - 1;
	xTitle.sBase.sPosition.sXMin = DISPLAY_TITLE_LEFT;
	xTitle.sBase.sPosition.sYMax = DISPLAY_TITLE_TOP + DISPLAY_TITLE_HEIGHT - 1;
	xTitle.sBase.sPosition.sYMin = DISPLAY_TITLE_TOP;
	xTitle.ulFillColor = DISPLAY_TITLE_BACKGROUND_COLOR;
	xTitle.ulOutlineColor = DISPLAY_TITLE_OUTLINE_COLOR;
	xTitle.ulStyle = DISPLAY_TITLE_STYLE;
	xTitle.ulTextColor = DISPLAY_TITLE_COLOR;
}

/**
 * Initialize the Title with the Definitions of displayStyle.h
 */
void vInitializeStatusMessage(void)
{
	xStatusMessage.pFont = DISPLAY_STATUS_MESSAGE_FONT;
	xStatusMessage.pcText = DISPLAY_STATUS_MESSAGE_TEXT;
	xStatusMessage.pucImage = NULL;
	xStatusMessage.sBase.lSize = sizeof(tPushButtonWidget);
	xStatusMessage.sBase.pChild = NULL;
	xStatusMessage.sBase.pDisplay = DISPLAY_DRIVER;
	xStatusMessage.sBase.pNext = NULL;
	xStatusMessage.sBase.pParent = NULL;
	xStatusMessage.sBase.pfnMsgProc = CanvasMsgProc;
	xStatusMessage.sBase.sPosition.sXMax = DISPLAY_STATUS_MESSAGE_LEFT
			+ DISPLAY_STATUS_MESSAGE_WIDTH - 1;
	xStatusMessage.sBase.sPosition.sXMin = DISPLAY_STATUS_MESSAGE_LEFT;
	xStatusMessage.sBase.sPosition.sYMax = DISPLAY_STATUS_MESSAGE_TOP
			+ DISPLAY_STATUS_MESSAGE_HEIGHT - 1;
	xStatusMessage.sBase.sPosition.sYMin = DISPLAY_STATUS_MESSAGE_TOP;
	xStatusMessage.ulFillColor = DISPLAY_STATUS_MESSAGE_BACKGROUND_COLOR;
	xStatusMessage.ulOutlineColor = DISPLAY_STATUS_MESSAGE_OUTLINE_COLOR;
	xStatusMessage.ulStyle = DISPLAY_STATUS_MESSAGE_STYLE;
	xStatusMessage.ulTextColor = DISPLAY_TITLE_COLOR;
}

void vShowStatusMessage(char* text)
{
	vInitializeStatusMessage();
	xStatusMessage.pcText = text;
	WidgetAdd((tWidget*) &xParentWidget, (tWidget*) &xStatusMessage);
	WidgetPaint((tWidget*) &xParentWidget);
}

void vRemoveStatusMessage(tBoolean bRefresh)
{
	WidgetRemove((tWidget*) &xStatusMessage);
	if (bRefresh == true)
	{
		WidgetPaint((tWidget*)&xParentWidget);
	}
}

/**
 * Set the Title Text, if NULL, the default from displayStyle.h will be used
 */
void vSetTitle(char* title)
{
	if (title == NULL)
	{
		xDisplayRoot.title = DISPLAY_TITLE_TEXT;
	}
	else
	{
		xDisplayRoot.title = title;
	}
	xTitle.pcText = xDisplayRoot.title;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

