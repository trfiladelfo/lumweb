/**
 * \addtogroup Tags
 * @{
 *
 * \file FloatInputField.c
 * \author Anziner, Hahn
 * \brief Routines for the FloatInputField tag
 *
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "setup.h"
#include "utils.h"

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/FloatInputField.h"


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

/**
 *
 * creates a float input field
 *
 */
void io_get_float_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1, decimal_place = 0;
	char *id = NULL, *label = NULL, *max = NULL, *min = NULL, *increment = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");
	max = SSIParamGetValue(*(params), "max");
	min = SSIParamGetValue(*(params), "min");
	increment = SSIParamGetValue(*(params), "increment");

	SSIParamDeleteAll(params);

	if (id != NULL && label != NULL) {
		if (min == NULL)
			min = "null";
		if (max == NULL)
			max = "null";
		if (increment == NULL)
			increment = "null";

		value = io_get_value_from_comtask(id);
		if (value != -1) {
			decimal_place = value % 10;
			value = value / 10;

			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ FloatInputField name=\"%s\" value=\"%d\" id=\"%s\" max=\"%s\" min=\"%s\" increment=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi\" name=\"f_%s\" value=\"%d.%d\" id=\"%s\" />"
						"<script>addB('%s',%s,%s,%s);</script>", label, (value * 10 + decimal_place), id,
					max, min, increment, label, id, value, decimal_place, id, id, max, min, increment);

#if DEBUG_SSI
			printf("io_get_number_input_field: done \n");
#endif
		} else {
#if DEBUG_SSI
			printf("io_get_number_input_field: queu error \n");
#endif
			snprintf(pcBuf, iBufLen,
					"NumberInputField: ERROR - NO DATA FROM QUEUE");
		}
	} else {
#if DEBUG_SSI
		printf("io_get_number_input_field: error no id and/or name found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"NumberInputField: ERROR - error no id and/or name found");
	}
}
