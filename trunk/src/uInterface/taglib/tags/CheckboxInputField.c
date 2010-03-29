/**
 * \addtogroup Tags
 * @{
 *
 * \file CheckboxInputField.c
 * \author Anziner, Hahn
 * \brief Routines for the CheckboxInputField tag
 *
*/

#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"
#include "taglib/tags/CheckboxInputField.h"
#include "ethernet/httpd/cgi/ssiparams.h"

#include "ethernet/httpd/cgi/io.h"


void vParseCheckboxInputField(char* param, int len, void* this) {
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
				vCreateNewEntity(xTagList + TAG_INDEX_CHECKBOXINPUTFIELD, id, name, NULL, iValue, -1, -1, -1);
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

/**
 *
 * generates the html code for an checkbox input field
 *
*/
void io_get_checkbox_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1;
	char *id = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");

	SSIParamDeleteAll(params);

	if (label != NULL && id != NULL) {
		value = io_get_value_from_comtask(id);
		if (value != -1) {
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ CheckboxInputField name=\"%s\" id=\"%s\" value=\"%d\" $ --> %s <input type=\"checkbox\" class=\"fi\" name=\"%s\" id=\"%s\" %s />",
					label, id, ((value != 0) ? 1 : 0), label, id, id, ((value
							!= 0) ? "checked=\"checked\"" : ""));
#if DEBUG_SSI
			printf("io_get_checkbox_input_field: done \n");
#endif
		} else {
#if DEBUG_SSI
			printf("io_get_checkbox_input_field: queu error \n");
#endif
			snprintf(pcBuf, iBufLen,
					"CheckboxInputField: ERROR - NO DATA FROM QUEUE");
		}
	} else {
#if DEBUG_SSI
		printf("io_get_checkbox_input_field: error no id and/or name found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"CheckboxInputField: ERROR - error no id and/or name found");
	}
}
