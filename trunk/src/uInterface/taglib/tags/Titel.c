/**
 * \addtogroup Tags
 * @{
 *
 * \file Titel.c
 * \author Anziner, Hahn
 * \brief Routines for the Titel tag
 *
*/

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/Titel.h"

void vParseTitle(char* param, int len, void* this) {
	char *label;
	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		xDisplayRoot.title = label;
		vSetTitle(label);
	} else {
#if DEBUG_HTTPC
		printf("vParseTitle: label NULL\n");
#endif
	}
}

/**
 *
 * creates a titel line
 *
 */
void io_get_titel(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL) {
		snprintf(pcBuf, iBufLen, "<!-- $ Titel label=\"%s\" $ -->"
			"<h1>%s</h1>", label, label);
	} else {
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
}
