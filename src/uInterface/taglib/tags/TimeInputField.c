/**
 * \addtogroup Tags
 * @{
 *
 * \file TimeInputField.c
 * \author Anziner, Hahn
 * \brief Routines for the TimeInputField tag
 *
*/

#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "taglib/tags/TimeInputField.h"


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

/**
 *
 * creates a time input field
 *
 */
void io_get_time_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1, hour = 0, minute = 0;
	char *id = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");

	SSIParamDeleteAll(params);

	printf("io_get_time_inpute_field: label=%s id=%s\n", label, id);

	if (id != NULL && label != NULL) {
		value = io_get_value_from_comtask(id);

		// parse value (in minutes)
		hour = value / 60;
		minute = value - (hour*60);

		if (value != -1) {
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ TimeInputField name=\"%s\" value=\"%d\" id=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi h\" name=\"t_%s\" value=\"%d\" id=\"%s_1\" />"
						":<input type=\"text\" class=\"fi m\" name=\"t_%s\" value=\"%d\" id=\"%s_2\" />"
						"<br><script>addBH('%s')</script>",
						label, value, id,
						label, id, hour, id, id, minute, id, id);

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
