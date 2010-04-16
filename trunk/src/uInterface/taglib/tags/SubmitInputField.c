/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Routines for the SubmitInputField tag
 *
 */

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "setup.h"

#include "graphic/gui/displayBasics.h"

#include "taglib/tags.h"
#include "taglib/tags/SubmitInputField.h"

/**
 *
 * creates submit input button
 *
 */
void vSubmitButtonRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL)
	{
		snprintf(pcBuf, iBufLen, "<!-- $ SubmitInputField label=\"%s\" $ -->"
			"<input type=\"submit\" value=\"%s\"/>", label, label);
	}
	else
	{
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
}

void vSubmitButtonOnLoad(char* param, int len, void* this)
{
	xDisplayRoot.save = true;
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

