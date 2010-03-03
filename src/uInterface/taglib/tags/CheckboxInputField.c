/*
 * FloatInputField.c
 *
 *  Created on: 02.03.2010
 *      Author: root
 */

#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"


void vParseCheckboxInputField(char* param, int len) {
	char *name, *value, *id;
	int iValue = 1;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	if (value == NULL || strcmp(value, "true") != 0 || strcmp(value, "on") != 0
			|| strcmp(value, "0") == 0) {
		iValue = 0;
	}

	if (id != NULL) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(xTagList[TAG_INDEX_CHECKBOXINPUTFIELD], id, name, NULL, iValue, -1, -1, -1);
			} else {
#if DEBUG_HTTPC
				printf("vParseCheckboxInputField: value NULL\n");
#endif
			}
		} else {
#if DEBUG_HTTPC
			printf("vParseCheckboxInputField: name NULL\n");
#endif
		}
	} else {
#if DEBUG_HTTPC
		printf("vParseCheckboxInputField: id NULL\n");
#endif
	}
}


