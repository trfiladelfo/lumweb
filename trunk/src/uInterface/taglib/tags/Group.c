/**
 * \addtogroup Tags
 * @{
 *
 * \file Group.c
 * \author Anziner, Hahn
 * \brief Routines for the Group tag
 *
*/

#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/Group.h"

void vParseGroup(char* param, int len) {
	char *label;

	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		vCreateNewEntity(xTagList + TAG_INDEX_GROUP, NULL, label, NULL, -1, -1, -1, -1);
	} else {
#if DEBUG_HTTPC
		printf("vParseIntegerInputField: label NULL\n");
#endif
	}

}

/**
 *
 * creates a group line
 *
 */
void io_get_group(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL) {
		snprintf(pcBuf, iBufLen, "<!-- $ Group label=\"%s\" $ -->"
			"<h3>%s</h3>", label, label);
	} else {
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
}


