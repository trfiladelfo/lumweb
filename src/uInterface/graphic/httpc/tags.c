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
#include "graphic/gui/displayBasics.h"
#include "graphic/gui/displayStyle.h"
#include "ethernet/httpd/cgi/io.h"
#include "configuration/configloader.h"

#include "setup.h"

char* pcGetParamFromString(char*, char*);
void vInserIntoList(basicDisplayLine*);
void vCreateNewEntity(int type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment);
char* pcFormatIntegerValue(basicDisplayLine *line);
char* pcFormatFloatValue(basicDisplayLine *line);
char* pcFormatTimeValue(basicDisplayLine *line);

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
#if DEBUG_HTTPC
				printf("vParseHyperlink: value NULL\n");
#endif
			}
		} else {
#if DEBUG_HTTPC
			printf("vParseHyperlink: name NULL\n");
#endif
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
#if DEBUG_HTTPC
		printf("vParseTitle: label NULL\n");
#endif
	}
}

void vParseGroup(char* param, int len) {
	char *label;

	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		vCreateNewEntity(SSI_INDEX_GROUP, NULL, label, NULL, -1, -1, -1, -1);
	} else {
#if DEBUG_HTTPC
		printf("vParseIntegerInputField: label NULL\n");
#endif
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

	// TO DO IMOLEMENT PARSE INCREMENT WITH DECIMAL POINT!
	//incrementStr = pcGetParamFromString(param, "increment");
	//if (incrementStr == NULL) {
		incr = 1;
	//} else {
	//	incr = atoi(incrementStr);
	//}
	if (id != NULL) {
		if (name != NULL) {
			if (value != NULL) {
				vCreateNewEntity(SSI_INDEX_FLOATINPUTFIELD, id, name, NULL,
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

char* pcGetParamFromString(char* str, char* search) {
	char *buffer, *retValue = NULL;
	int i, j, len;

#if DEBUG_HTTPC
	printf("pcGetParamFromString str=%s, search=%s,\n", str, search);
#endif

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

#if DEBUG_HTTPC
		printf("vInsertIntoList: root\n");
#endif
	} else {
		while (akt->next != NULL) {
			akt = akt->next;
		}
		akt->next = toInsert;

#if DEBUG_HTTPC
		printf("vInsertIntoList: at the End\n");
#endif

		xDisplayRoot.displayEntities = true;
	}
}

void vCreateNewEntity(int type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment) {
	basicDisplayLine *newLine = pvPortMalloc(sizeof(basicDisplayLine));
	newLine->type = type;
	newLine->id = id;
	newLine->label = label;
	newLine->value = value;
	newLine->max = max;
	newLine->min = min;
	newLine->increment = increment;
	switch (type) {
	case SSI_INDEX_INTEGERINPUTFIELD:
		pcFormatIntegerValue(newLine);
		break;
	case SSI_INDEX_FLOATINPUTFIELD:
		pcFormatFloatValue(newLine);
		break;
	case SSI_INDEX_TIMEINPUTFIELD:
		pcFormatTimeValue(newLine);
		break;
	default:
		strcpy(newLine->strValue, strValue);
		break;
	}
	newLine->next = NULL;
	newLine->labelWidget = NULL;
	newLine->valueWidget = NULL;
	vInserIntoList(newLine);
}

char* pcFormatIntegerValue(basicDisplayLine *line) {
	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%d", line->value);
	return line->strValue;
}

char* pcFormatFloatValue(basicDisplayLine *line) {
	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%d,%d",
			(line->value / 10), iAbs((line->value % 10)));
	return line->strValue;
}

char* pcFormatTimeValue(basicDisplayLine *line) {
	snprintf(line->strValue, DISPLAY_VALUE_TEXT_LEN, "%02d:%02d", line->value
			/ 60, line->value - ((line->value / 60) * 60));
	return line->strValue;
}
