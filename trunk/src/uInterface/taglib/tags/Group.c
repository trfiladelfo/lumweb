
#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

void vParseGroup(char* param, int len) {
	char *label;

	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		vCreateNewEntity(xTagList[TAG_INDEX_GROUP], NULL, label, NULL, -1, -1, -1, -1);
	} else {
#if DEBUG_HTTPC
		printf("vParseIntegerInputField: label NULL\n");
#endif
	}

}

