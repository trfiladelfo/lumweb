
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"
void vParseIntegerInputField(char* param, int len) {
	char *name, *value, *id, *maxStr, *minStr, *incrementStr;

	int max, min, incr;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");
	maxStr = pcGetParamFromString(param, "max");
	if (maxStr == NULL) {
		max = -1;
	} else {
		max = atoi(maxStr);
	}
	minStr = pcGetParamFromString(param, "min");
	if (minStr == NULL) {
		min = -1;
	} else {
		min = atoi(minStr);
	}
	incrementStr = pcGetParamFromString(param, "increment");
	if (incrementStr == NULL) {
		incr = 1;
	} else {
		incr = atoi(incrementStr);
	}

	if (id != NULL) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(xTagList[TAG_INDEX_INTEGERINPUTFIELD], id, name, NULL,
						atoi(value), max, min, incr);
			} else {
#if DEBUG_HTTPC
				printf("vParseIntegerInputField: value NULL\n");
#endif
			}
		} else {
#if DEBUG_HTTPC
			printf("vParseIntegerInputField: name NULL\n");
#endif
		}
	} else {
#if DEBUG_HTTPC
		printf("vParseIntegerInputField: id NULL\n");
#endif
	}
}

char* pcFormatIntegerValue(basicDisplayLine *line) {
	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%d", line->value);
	return line->strValue;
}
