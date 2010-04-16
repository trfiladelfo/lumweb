/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Routines for the IntegerInputField tag
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"

#include "setup.h"

#include "taglib/tags.h"
#include "taglib/tags/IntegerInputField.h"

#include "ethernet/httpd/cgi/io.h"
#include "ethernet/httpd/cgi/ssiparams.h"

#include "graphic/gui/touchActions.h"

void vIntegerOnLoad(char* param, int len, void* this)
{
	char *name, *value, *id, *maxStr, *minStr, *incrementStr;

	int max, min, incr;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");
	maxStr = pcGetParamFromString(param, "max");
	if (maxStr == NULL)
	{
		max = -1;
	}
	else
	{
		max = atoi(maxStr);
	}
	minStr = pcGetParamFromString(param, "min");
	if (minStr == NULL)
	{
		min = -1;
	}
	else
	{
		min = atoi(minStr);
	}
	incrementStr = pcGetParamFromString(param, "increment");
	if (incrementStr == NULL)
	{
		incr = 1;
	}
	else
	{
		incr = atoi(incrementStr);
	}

	if (id != NULL)
	{
		if (name != NULL)
		{
			if (value != NULL)
			{
				vCreateNewEntity(xTagList + TAG_INDEX_INTEGERINPUTFIELD, id,
						name, NULL, atoi(value), max, min, incr);
			}
			else
			{
#if DEBUG_TAGS
				printf("vIntegerOnLoad: value NULL\n");
#endif
			}
		}
		else
		{
#if DEBUG_TAGS
			printf("vIntegerOnLoad: name NULL\n");
#endif
		}
	}
	else
	{
#if DEBUG_TAGS
		printf("vIntegerOnLoad: id NULL\n");
#endif
	}
}

char* pcIntegerStrFormatter(void*line)
{
	snprintf(((basicDisplayLine*) line)->strValue, DISPLAY_VALUE_TEXT_LEN,
			"%d", ((basicDisplayLine*) line)->value);
	return ((basicDisplayLine*) line)->strValue;
}

/**
 *
 * creates number input field  and +/- buttons
 *
 */

void vIntegerRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	int value = 1;
	char *id = NULL, *label = NULL, *max = NULL, *min = NULL, *decimal = NULL,
			*increment = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");
	max = SSIParamGetValue(*(params), "max");
	min = SSIParamGetValue(*(params), "min");
	decimal = SSIParamGetValue(*(params), "decimal");
	increment = SSIParamGetValue(*(params), "increment");

	SSIParamDeleteAll(params);

	if (id != NULL && label != NULL)
	{
		if (min == NULL)
			min = "-1";
		if (max == NULL)
			max = "-1";
		if (decimal == NULL)
			decimal = "0";
		if (increment == NULL)
			increment = "1";

		value = io_get_value_from_comtask(id);
		if (value != -1)
		{
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ IntegerInputField name=\"%s\" value=\"%d\" id=\"%s\" max=\"%s\" min=\"%s\" increment=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi\" name=\"%s\" value=\"%d\" id=\"%s\" />"
						"<script>addB('%s',%s,%s,%s);</script>", label, value,
					id, max, min, increment, label, id, value, id, id, max,
					min, increment);

#if DEBUG_TAGS
			printf("vIntegerRenderSSI: done \n");
#endif
		}
		else
		{
#if DEBUG_TAGS
			printf("vIntegerRenderSSI: queu error \n");
#endif
			snprintf(pcBuf, iBufLen,
					"NumberInputField: ERROR - NO DATA FROM QUEUE");
		}
	}
	else
	{
#if DEBUG_TAGS
		printf("vIntegerRenderSSI: error no id and/or name found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"NumberInputField: ERROR - error no id and/or name found");
	}
}

tWidget* xIntegerOnDisplay(void* this, int row)
{
	basicDisplayLine* line = (basicDisplayLine*) this;

	if (line->valueWidget != NULL)
	{
		vPortFree(line->valueWidget);
		line->valueWidget = NULL;
	}

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

	((tPushButtonWidget*) line->valueWidget)->pfnOnClick = vOpenEditorAction;
	((tPushButtonWidget*) line->valueWidget)->pcText = line->strValue;

	return (tWidget*) line->valueWidget;
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

