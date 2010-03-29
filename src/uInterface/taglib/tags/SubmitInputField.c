/**
 * \addtogroup Tags
 * @{
 *
 * \file SubmitInputField.c
 * \author Anziner, Hahn
 * \brief Routines for the SubmitInputField tag
 *
*/

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/SubmitInputField.h"

/**
 *
 * creates submit input button
 *
 */
void io_get_submit_input_button(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL) {
		//if(ajax_id != NULL) // AJAX
		snprintf(pcBuf, iBufLen, "<!-- $ SubmitInputField label=\"%s\" $ -->"
			"<input type=\"submit\" value=\"%s\"/>", label, label);
		//else
		//	snprintf(pcBuf, iBufLen, "<!-- $ SubmitInputField label=%s $ -->"
		//			"<input type=\"submit\" name=\"%s\" value=\"%s\" />", label, label, label);
	} else {
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
}
