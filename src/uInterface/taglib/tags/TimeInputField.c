/**
 * \addtogroup Tags
 * @{
 *
 * \file TimeInputField.c
 * \author Anziner, Hahn
 * \brief Routines for the TimeInputField tag
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"

#include "setup.h"

#include "taglib/taglib.h"
#include "taglib/tags.h"
#include "taglib/tags/TimeInputField.h"

#include "ethernet/httpd/cgi/io.h"

#include "graphic/gui/touchActions.h"

void vTimeOnLoad(char* param, int len, void* this)
{
	char *name, *value, *id;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	if (id != NULL)
	{
		if (name != NULL)
		{
			if (value != NULL)
			{
				vCreateNewEntity(xTagList + TAG_INDEX_TIMEINPUTFIELD, id, name,
						NULL, atoi(value), -1, -1, -1);
			}
			else
			{
#if DEBUG_TAGS
				printf("vTimeOnLoad: value NULL\n");
#endif
			}
		}
		else
		{
#if DEBUG_TAGS
			printf("vTimeOnLoad: name NULL\n");
#endif
		}
	}
	else
	{
#if DEBUG_TAGS
		printf("vTimeOnLoad: id NULL\n");
#endif
	}
}

char* pcTimeStrFormatter(void* line)
{
	snprintf(
			(((basicDisplayLine*) line)->strValue),
			DISPLAY_VALUE_TEXT_LEN,
			"%02d:%02d",
			(((basicDisplayLine*) line)->value / 60),
			(((basicDisplayLine*) line)->value % 60)
	);
	return ((basicDisplayLine*) line)->strValue;
}

/**
 *
 * creates a time input field
 *
 */

void vTimeRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	int value = 1, hour = 0, minute = 0;
	char *id = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");

	SSIParamDeleteAll(params);

#if DEBUG_TAGS
	printf("vTimeRenderSSI: label=%s id=%s\n", label, id);
#endif

	if (id != NULL && label != NULL)
	{
		value = io_get_value_from_comtask(id);

		// parse value (in minutes)
		hour = value / 60;
		minute = value - (hour * 60);

		if (value != -1)
		{
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ TimeInputField name=\"%s\" value=\"%d\" id=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi h\" name=\"t_%s\" value=\"%d\" id=\"%s_1\" />"
						":<input type=\"text\" class=\"fi m\" name=\"t_%s\" value=\"%d\" id=\"%s_2\" />"
						"<br><script>addBH('%s')</script>", label, value, id,
					label, id, hour, id, id, minute, id, id);

#if DEBUG_TAGS
			printf("vTimeRenderSSI: done \n");
#endif
		}
		else
		{
#if DEBUG_TAGS
			printf("vTimeRenderSSI: queue error \n");
#endif
			snprintf(pcBuf, iBufLen,
					"NumberInputField: ERROR - NO DATA FROM QUEUE");
		}
	}
	else
	{
#if DEBUG_TAGS
		printf("vTimeRenderSSI: error no id and/or name found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"NumberInputField: ERROR - error no id and/or name found");
	}
}

tWidget* xTimeOnDisplay(void* this, int row)
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
