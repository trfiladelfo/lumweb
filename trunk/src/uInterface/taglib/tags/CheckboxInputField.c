/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Routines for the CheckboxInputField tag
 *
 */

#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"

#include "setup.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/checkbox.h"

#include "taglib/tags.h"
#include "taglib/tags/CheckboxInputField.h"

#include "ethernet/httpd/cgi/ssiparams.h"
#include "ethernet/httpd/cgi/io.h"

#include "graphic/gui/touchActions.h"

void vCheckboxOnLoad(char* param, int len, void* this)
{
	char *name, *value, *id;
	int iValue = 1;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	if (value == NULL || strcmp(value, "true") != 0 || strcmp(value, "on") != 0
			|| strcmp(value, "0") == 0)
	{
		iValue = 0;
	}

	if (id != NULL)
	{
		if (name != NULL)
		{
			if (value != NULL)
			{
				vCreateNewEntity(xTagList + TAG_INDEX_CHECKBOXINPUTFIELD, id,
						name, NULL, iValue, -1, -1, -1);
			}
			else
			{
#if DEBUG_TAGS
				printf("vCheckboxOnLoad: value NULL\n");
#endif
			}
		}
		else
		{
#if DEBUG_TAGS
			printf("vCheckboxOnLoad: name NULL\n");
#endif
		}
	}
	else
	{
#if DEBUG_TAGS
		printf("vCheckboxOnLoad: id NULL\n");
#endif
	}
}

/**
 *
 * generates the html code for an checkbox input field
 *
 */
void vCheckboxRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	int value = 1;
	char *id = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");

	SSIParamDeleteAll(params);

	if (label != NULL && id != NULL)
	{
		value = io_get_value_from_comtask(id);
		if (value != -1)
		{
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ CheckboxInputField name=\"%s\" id=\"%s\" value=\"%d\" $ --> %s <input type=\"checkbox\" class=\"fi\" name=\"%s\" id=\"%s\" %s />",
					label, id, ((value != 0) ? 1 : 0), label, id, id, ((value
							!= 0) ? "checked=\"checked\"" : ""));
#if DEBUG_TAGS
			printf("vCheckboxRenderSSI: done \n");
#endif
		}
		else
		{
#if DEBUG_TAGS
			printf("vCheckboxRenderSSI: queu error \n");
#endif
			snprintf(pcBuf, iBufLen,
					"CheckboxInputField: ERROR - NO DATA FROM QUEUE");
		}
	}
	else
	{
#if DEBUG_TAGS
		printf("vCheckboxRenderSSI: error no id and/or name found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"CheckboxInputField: ERROR - error no id and/or name found");
	}
}

tWidget* xCheckboxOnDisplay(void* this, int row)
{
	basicDisplayLine* line = (basicDisplayLine*) this;

	if (line->valueWidget != NULL)
	{
		vPortFree(line->valueWidget);
		line->valueWidget = NULL;
	}

	line->valueWidget = pvPortMalloc(sizeof(tCheckBoxWidget));

	((tCheckBoxWidget*) line->valueWidget)->ulFillColor
			= DISPLAY_VALUE_CHECKBOX_BACKGROUND_COLOR;
	((tCheckBoxWidget*) line->valueWidget)->pcText = NULL;
	((tCheckBoxWidget*) line->valueWidget)->pfnOnChange = vCheckboxAction;
	((tCheckBoxWidget*) line->valueWidget)->pucImage = NULL;
	((tCheckBoxWidget*) line->valueWidget)->ulOutlineColor
			= DISPLAY_VALUE_CHECKBOX_OUTLINE_COLOR;
	((tCheckBoxWidget*) line->valueWidget)->ulTextColor = DISPLAY_VALUE_COLOR;
	((tCheckBoxWidget*) line->valueWidget)->usBoxSize = DISPLAY_LINE_HEIGHT;
	((tCheckBoxWidget*) line->valueWidget)->usStyle
			= DISPLAY_VALUE_STYLE_BOOLEAN;

	if (line->value != 0)
	{
		((tCheckBoxWidget*) line->valueWidget)->usStyle |= CB_STYLE_SELECTED;
	}
	((tCheckBoxWidget*) line->valueWidget)->sBase.lSize
			= sizeof(tCheckBoxWidget);
	((tCheckBoxWidget*) line->valueWidget)->sBase.pChild = NULL;
	((tCheckBoxWidget*) line->valueWidget)->sBase.pDisplay = DISPLAY_DRIVER;
	((tCheckBoxWidget*) line->valueWidget)->sBase.pNext = NULL;
	((tCheckBoxWidget*) line->valueWidget)->sBase.pParent = NULL;
	((tCheckBoxWidget*) line->valueWidget)->sBase.pfnMsgProc = CheckBoxMsgProc;
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

	return line->valueWidget;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

