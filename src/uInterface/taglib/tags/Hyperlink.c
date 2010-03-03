
#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "configuration/configloader.h"

void vParseHyperlink(char* param, int len) {

	char *name, *value;

	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");
	if (strcmp(configLoad, value) != 0) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(xTagList[TAG_INDEX_HYPERLINK], NULL, name, value, -1,
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
