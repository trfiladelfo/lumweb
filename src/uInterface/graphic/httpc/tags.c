/*
 * integerInputField.c
 *
 *  Created on: 19.02.2010
 *      Author: root
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "utils.h"

#include "hw_types.h"

#include "tags.h"
#include "graphic/gui/dislpayBasics.h"
#include "ethernet/httpd/cgi/io.h"
#include "configuration/configloader.h"

char* pcGetParamFromString(char*, char*);
void vInserIntoList(basicDisplayLine*);
void vCreateNewEntity(int type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment);

void vParseHyperlink(char* param, int len) {

	char *name, *value;

	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");
	if (strcmp(configLoad, value) != 0) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(SSI_INDEX_HYPERLINK, NULL, name, value, -1,
						-1, -1, -1);
			} else {
				printf("vParseHyperlink: value NULL\n");
			}
		} else {
			printf("vParseHyperlink: name NULL\n");
		}
	} else {
		xDisplayRoot.menue = true;
	}
	vPortFree(configLoad);
}

void vParseTitle(char* param, int len) {
	char *label;
	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		xDisplayRoot.title = label;
		vSetTitle(label);
	} else {
		printf("vParseTitle: label NULL\n");
	}
}

void vParseGroup(char* param, int len) {
	char *label;

	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		vCreateNewEntity(SSI_INDEX_GROUP, NULL, label, NULL, -1, -1, -1, -1);
	} else {
		printf("vParseIntegerInputField: label NULL\n");
	}

}

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
				vCreateNewEntity(SSI_INDEX_INTEGERINPUTFIELD, id, name, NULL,
						atoi(value), max, min, incr);
			} else {
				printf("vParseIntegerInputField: value NULL\n");
			}
		} else {
			printf("vParseIntegerInputField: name NULL\n");
		}
	} else {
		printf("vParseIntegerInputField: id NULL\n");
	}
}

void vParseFloatInputField(char* param, int len) {
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
				vCreateNewEntity(SSI_INDEX_FLOATINPUTFIELD, id, name, NULL,
						atoi(value), max, min, incr);
			} else {
				printf("vParseFloatInputField: value NULL\n");
			}
		} else {
			printf("vParseFloatInputField: name NULL\n");
		}
	} else {
		printf("vParseFloatInputField: id NULL\n");
	}
}
void vParseTimeInputField(char* param, int len) {
	char *name, *value, *id;

	id = pcGetParamFromString(param, "id");
	name = pcGetParamFromString(param, "name");
	value = pcGetParamFromString(param, "value");

	if (id != NULL) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(SSI_INDEX_TIMEINPUTFIELD, id, name, NULL,
						atoi(value), -1, -1, -1);
			} else {
				printf("vParseTimeInputField: value NULL\n");
			}
		} else {
			printf("vParseTimeInputField: name NULL\n");
		}
	} else {
		printf("vParseTimeInputField: id NULL\n");
	}
}

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
				vCreateNewEntity(SSI_INDEX_CHECKBOXINPUTFIELD, id, name, NULL,
						iValue, -1, -1, -1);
			} else {
				printf("vParseCheckboxInputField: value NULL\n");
			}
		} else {
			printf("vParseCheckboxInputField: name NULL\n");
		}
	} else {
		printf("vParseCheckboxInputField: id NULL\n");
	}
}

char* pcGetParamFromString(char* str, char* search) {
	char *buffer, *retValue = NULL;
	int i, j, len;
	printf("pcGetParamFromString str=%s, search=%s,\n", str, search);
	buffer = pcStrstr(str, search);
	if (buffer != NULL) {

		buffer = pcStrstr(buffer, "=\"");
		buffer += 2;

		for (len = 0; buffer[len] != '\"'; len++)
			;

		retValue = pvPortMalloc((len + 1) * sizeof(char));

		j = 0;
		for (i = 0; i < len; i++) {
			retValue[j] = buffer[i];
			j++;
		}
		retValue[j] = 0;
	}

	return retValue;
}

void vInserIntoList(basicDisplayLine *toInsert) {

	basicDisplayLine *akt = xDisplayRoot.entities;
	if (akt == NULL || xDisplayRoot.displayEntities == false) {
		xDisplayRoot.entities = toInsert;
		xDisplayRoot.displayEntities = true;
		printf("vInsertIntoList: root\n");
	} else {
		while (akt->next != NULL) {
			akt = akt->next;
		}
		akt->next = toInsert;
		printf("vInsertIntoList: at the End\n");
		xDisplayRoot.displayEntities = true;
	}
}

void vCreateNewEntity(int type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment) {
	basicDisplayLine *newLine = pvPortMalloc(sizeof(basicDisplayLine));
	newLine->type = type;
	newLine->id = id;
	newLine->label = label;
	newLine->strValue = strValue;
	newLine->value = value;
	newLine->max = max;
	newLine->min = min;
	newLine->increment = increment;
	newLine->next = NULL;
	newLine->labelWidget = NULL;
	newLine->valueWidget = NULL;
	vInserIntoList(newLine);
}
