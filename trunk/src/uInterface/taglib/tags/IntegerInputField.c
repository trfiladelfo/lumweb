/**
 * \addtogroup Tags
 * @{
 *
 * \file IntegerInputField.c
 * \author Anziner, Hahn
 * \brief Routines for the IntegerInputField tag
 *
*/


#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/IntegerInputField.h"

#include "ethernet/httpd/cgi/io.h"
#include "ethernet/httpd/cgi/ssiparams.h"



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

/**
 *
 * creates number input field  and +/- buttons
 *
 */
void io_get_integer_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1;
	char *id = NULL, *label = NULL, *max = NULL, *min = NULL, *decimal = NULL, *increment = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");
	max = SSIParamGetValue(*(params), "max");
	min = SSIParamGetValue(*(params), "min");
	decimal = SSIParamGetValue(*(params), "decimal");
	increment = SSIParamGetValue(*(params), "increment");

	SSIParamDeleteAll(params);

	if (id != NULL && label != NULL) {
		if (min == NULL)
			min = "-1";
		if (max == NULL)
			max = "-1";
		if (decimal == NULL)
			decimal = "0";
		if (increment == NULL)
			increment = "1";

		value = io_get_value_from_comtask(id);
		if (value != -1) {
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ IntegerInputField name=\"%s\" value=\"%d\" id=\"%s\" max=\"%s\" min=\"%s\" increment=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi\" name=\"%s\" value=\"%d\" id=\"%s\" />"
						"<script>addB('%s',%s,%s,%s);</script>", label, value, id,
					max, min, increment, label, id, value, id, id, max, min, increment);

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
