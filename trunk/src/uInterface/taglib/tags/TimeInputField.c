
#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"
void vParseTimeInputField(char* param, int len) {
	char *name, *value, *id;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	if (id != NULL) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(xTagList[TAG_INDEX_TIMEINPUTFIELD], id, name, NULL,
						atoi(value), -1, -1, -1);
			} else {
#if DEBUG_HTTPC
				printf("vParseTimeInputField: value NULL\n");
#endif
			}
		} else {
#if DEBUG_HTTPC
			printf("vParseTimeInputField: name NULL\n");
#endif
		}
	} else {
#if DEBUG_HTTPC
		printf("vParseTimeInputField: id NULL\n");
#endif
	}
}


char* pcFormatTimeValue(basicDisplayLine *line) {
	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%02d:%02d", line->value
			/ 60, line->value - ((line->value / 60) * 60));
	return line->strValue;
}
