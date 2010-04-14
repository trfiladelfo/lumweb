/**
 * \addtogroup Tags
 * @{
 *
 * \file Hyperlink.c
 * \author Anziner, Hahn
 * \brief Routines for the Hyperlink tag
 *
 */

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"

#include "setup.h"

#include "taglib/tags.h"
#include "taglib/tags/Hyperlink.h"

#include "configuration/configloader.h"

#include "graphic/gui/touchActions.h"

void vHyperlinkOnLoad(char* param, int len, void* this)
{

	char *name, *value;

	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");
	if (strcmp(configLoad, value) != 0)
	{
		if (name != NULL)
		{
			if (value != NULL)
			{
				vCreateNewEntity(xTagList + TAG_INDEX_HYPERLINK, NULL, name,
						value, -1, -1, -1, -1);
			}
			else
			{
#if DEBUG_TAGS
				printf("vHyperlinkOnLoad: value NULL\n");
#endif
			}
		}
		else
		{
#if DEBUG_TAGS
			printf("vHyperlinkOnLoad: name NULL\n");
#endif
		}
	}
	else
	{
		xDisplayRoot.menue = true;
	}
	vPortFree(configLoad);
}

void vHyperlinkRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	char *value = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	value = SSIParamGetValue(*(params), "value");

	SSIParamDeleteAll(params);

	if (label != NULL && value != NULL)
	{
		snprintf(
				pcBuf,
				iBufLen,
				"<!-- $ Hyperlink name=\"%s\" value=\"%s\" $ --> <a href=\"%s\">%s</a>",
				label, value, value, label);
#if DEBUG_TAGS
		printf("vHyperlinkRenderSSI: done \n");
#endif
	}
	else
	{
#if DEBUG_TAGS
		printf("vHyperlinkRenderSSI: error no id and/or value found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"Hyperlink: ERROR - error no id and/or value found");
	}
}

char* pcHyperlinkStrFormatter(void* this)
{
	return ((basicDisplayLine*) this)->strValue;
}

tWidget* xHyperlinkOnDisplay(void* this, int row)
{
	basicDisplayLine* line = (basicDisplayLine*) this;

	if (line->valueWidget != NULL)
	{
		vPortFree(line->valueWidget);
		line->valueWidget = NULL;
	}

	printf("xHyperlinkOnDisplay: new Hyperlink created\n");

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
			= DISPLAY_VALUE_PUSH_COLOR;
	((tPushButtonWidget*) line->valueWidget)->ulStyle
			= DISPLAY_VALUE_STYLE_HYPERLINK;
	((tPushButtonWidget*) line->valueWidget)->ulTextColor = DISPLAY_VALUE_COLOR;
	((tPushButtonWidget*) line->valueWidget)->usAutoRepeatDelay = 0;
	((tPushButtonWidget*) line->valueWidget)->usAutoRepeatRate = 0;
	((tPushButtonWidget*) line->valueWidget)->sBase.lSize
			= sizeof(tPushButtonWidget);
	((tPushButtonWidget*) line->valueWidget)->sBase.pChild = NULL;
	((tPushButtonWidget*) line->valueWidget)->sBase.pDisplay = DISPLAY_DRIVER;
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

	((tPushButtonWidget*) line->valueWidget)->pfnOnClick = vHyperlinkAction;
	((tPushButtonWidget*) line->valueWidget)->pcText
			= DISPLAY_VALUE_TEXT_HYPERLINK;

	return (tWidget*) line->valueWidget;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
