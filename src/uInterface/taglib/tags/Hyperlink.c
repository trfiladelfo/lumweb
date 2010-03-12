/**
 * \addtogroup Tags
 * @{
 *
 * \file Hyperlink.c
 * \author Anziner, Hahn
 * \brief Routines for the Hyperlink tag
 *
*/


#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/Hyperlink.h"

#include "configuration/configloader.h"

void vParseHyperlink(char* param, int len) {

	char *name, *value;

	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");
	if (strcmp(configLoad, value) != 0) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(xTagList + TAG_INDEX_HYPERLINK, NULL, name, value, -1,
						-1, -1, -1);
			} else {
#if DEBUG_HTTPC
				printf("vParseHyperlink: value NULL\n");
#endif
			}
		} else {
#if DEBUG_HTTPC
			printf("vParseHyperlink: name NULL\n");
#endif
		}
	} else {
		xDisplayRoot.menue = true;
	}
	vPortFree(configLoad);
}

//*****************************************************************************
//
//
//
//*****************************************************************************
void io_get_hyperlink(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *value = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	value = SSIParamGetValue(*(params), "value");

	SSIParamDeleteAll(params);

	if (label != NULL && value != NULL) {
		snprintf(
				pcBuf,
				iBufLen,
				"<!-- $ Hyperlink name=\"%s\" value=\"%s\" $ --> <a href=\"%s\">%s</a>",
				label, value, value, label);
#if DEBUG_SSI
		printf("io_get_hyperlink: done \n");
#endif
	} else {
#if DEBUG_SSI
		printf("io_get_hyperlink: error no id and/or value found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"Hyperlink: ERROR - error no id and/or value found");
	}
}
