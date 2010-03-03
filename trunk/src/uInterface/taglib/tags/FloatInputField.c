/*
 * FloatInputField.c
 *
 *  Created on: 02.03.2010
 *      Author: root
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "setup.h"
#include "utils.h"

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

void vParseFloatInputField(char* param, int len) {
	char *name, *value, *id, *maxStr, *minStr; // *incrementStr;

	int max, min, incr;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");
	maxStr = pcGetParamFromString(param, "max");
	if (maxStr == NULL) {
		max = -1;
	} else {
		max = atoi(maxStr)*10;
	}
	minStr = pcGetParamFromString(param, "min");
	if (minStr == NULL) {
		min = -1;
	} else {
		min = atoi(minStr)*10;
	}

	// TODO IMOLEMENT PARSE INCREMENT WITH DECIMAL POINT!
	//incrementStr = pcGetParamFromString(param, "increment");
	//if (incrementStr == NULL) {
		incr = 1;
	//} else {
	//	incr = atoi(incrementStr);
	//}
	if (id != NULL) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity( xTagList[TAG_INDEX_FLOATINPUTFIELD], id, name, NULL,
						atoi(value), max, min, incr);
			} else {
#if DEBUG_HTTPC
				printf("vParseFloatInputField: value NULL\n");
#endif
			}
		} else {
#if DEBUG_HTTPC
			printf("vParseFloatInputField: name NULL\n");
#endif
		}
	} else {
#if DEBUG_HTTPC
		printf("vParseFloatInputField: id NULL\n");
#endif
	}
}

char* pcFormatFloatValue(basicDisplayLine *line) {
	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%d,%d",
			(line->value / 10), iAbs((line->value % 10)));
	return line->strValue;
}

