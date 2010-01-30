/*
 * graphicWebClient.c
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#include <string.h>
#include "graphic/graphicWebClient.h"
#include "graphic/graphicLib.h"
#include "graphic/graphicSettings.h"
#include "graphic/touchActions.h"

#include "configuration/configloader.h"

#include "ethernet/LWIPStack.h"

#include "lwip/netbuf.h"
#include "lwip/api.h"

#include "utils.h"

#define STARTTAG_LEN 5
const char* STARTTAG = "<!--$";
const char* ENDTAG = ">";

struct netconn *conn;
struct netbuf *inBuf;
char *pageData;

void drawGWC();
void addRow(char* id, char* name, long value, char status);
void addHTMLToList(char* str, int len);
char* getValueForParamName(char* str, char* search, char* retValue);
char *getElementType(char * str, char *buff);
void sendData(tWidget *pWidget);

int aktPage = 1;
char* aktWebPage = NULL;

void loadMenu(void) {
	char* configLoad = loadFromConfig(IP_CONFIG_FILE, "DEFAULT_MENU_PAGE");
	loadPage(configLoad);
	vPortFree(configLoad);
}

void addRow(char* id, char* name, long value, char status) {

	gwcRow *newRow = (gwcRow*) pvPortMalloc(sizeof(gwcRow));
	newRow->id = id;
	newRow->name = name;
	newRow->value = value;
	newRow->status = status;
	newRow->next = NULL;
	newRow->stringValue = NULL;
	newRow->checkbox = NULL;
	newRow->decrease = NULL;
	newRow->increase = NULL;
	newRow->nameLabel = NULL;
	newRow->valueLabel = NULL;

	insertGWC(newRow);
}

void insertGWC(gwcRow* toInsert) {
	gwcRow* akt = pgwcRoot;
	if (akt == NULL) {
		pgwcRoot = toInsert;
	} else {
		while (akt->next != NULL) {
			akt = akt->next;
		}
		akt->next = toInsert;
	}
}

void drawGWC() {

	int offset = (aktPage - 1) * GWC_ROWS_PER_VIEW;
	gwcRow *akt = pgwcRoot;

	int aktRow = 0;
	int top = GWC_FIRST_ROW_TOP;
	int i;

	if (offset > 0) {
		for (i = 0; akt->next != NULL && i < offset; i++) {
			akt = akt->next;
		}
	} else if (offset < 0) {
		offset = 0;
	}

	initPanel("LumWeb - The Universal Interface");
	cleanDisplay();

	for (i = 0; akt != NULL && i < GWC_ROWS_PER_VIEW; i++) {

		if ((akt->status & GWC_LABEL) == GWC_LABEL) {

			akt->nameLabel = addLabel(GWC_ROW_LABEL_LEFT, top,
					GWC_ROW_LABEL_WIDTH, GWC_ROW_HEIGHT, akt->name);
		}

		if ((akt->status & GWC_NUMERIC) == GWC_NUMERIC) {

			if (!akt->stringValue) {
				akt->stringValue = (char*) pvPortMalloc(
						(GWC_ROW_VALUE_MAX_LENGTH + 1) * sizeof(char));
			}
			akt->decrease = addButton(GWC_ROW_DECREASE_BUTTON_LEFT, top,
					GWC_ROW_DECREASE_BUTTON_WIDTH, GWC_ROW_HEIGHT,
					GWC_ROW_DECREASE_BUTTON_SYMBOL,
					GWC_ROW_DECREASE_AUTOREPEAT, decrease);
			snprintf(akt->stringValue, (GWC_ROW_VALUE_MAX_LENGTH + 1), "%d",
					akt->value);
			akt->valueLabel = addLabel(GWC_ROW_VALUE_LEFT, top,
					GWC_ROW_VALUE_WIDTH, GWC_ROW_HEIGHT, akt->stringValue);
			akt->increase = addButton(GWC_ROW_INCREASE_BUTTON_LEFT, top,
					GWC_ROW_INCREASE_BUTTON_WIDTH, GWC_ROW_HEIGHT,
					GWC_ROW_INCREASE_BUTTON_SYMBOL,
					GWC_ROW_INCREASE_AUTOREPEAT, increase);

		} else if (akt->status & GWC_BOOLEAN == GWC_BOOLEAN) {
			akt->checkbox = addCheckbox(GWC_ROW_CHECKBOX_LEFT, top,
					GWC_ROW_CHECKBOX_WIDTH, GWC_ROW_HEIGHT,
					(unsigned long) akt->value, onCheckboxClick);

		}

		akt = akt->next;
		top += GWC_ROW_HEIGHT + GWC_ROW_MARGIN_TOP;

	}

	if (offset > 0) {
		addButton(5, 205, 80, 30, "Back", 0, backPage);
	}

	addButton(90, 205, 140, 30, "Speichern", 0, sendData);

	if (akt != NULL && akt->next != NULL) {
		addButton(235, 205, 80, 30, "Continue", 0, continuePage);
	}

	drawPanel();
	printf("Ouput erfolgreich\n");
}

void loadPage(char *uri) {
	tBoolean tagOpen = pdFALSE;

	char buffer[128];
	char inTag = 0;

	// status variables
	u16_t length, bindErr, connErr, writeErr;
	u16_t port;

	aktPage = 1;
	// Set the get String

	sprintf(buffer, "GET /%s HTTP/1.0\r\n\r\n", uri);
	printf(buffer);

	// create new connection
	conn = netconn_new(NETCONN_TCP);

	// loop counter
	int i = 0, bufferPos;

	// create a connection
	connErr = netconn_connect(conn, remoteIP, 80);
	if (conn != NULL && connErr == 0) {

		// send request
		writeErr = netconn_write(conn, &buffer, sizeof(buffer), NETCONN_NOCOPY);

		// recieve answer
		inBuf = netconn_recv(conn);
		inTag = 0;
		while (inBuf != NULL) {
			do {
				// read data
				netbuf_data(inBuf, (void**) &pageData, &length);

				for (i = 0; i < length; i++) {
					if (pageData[i] == '$') {
						inTag = (inTag != 0) ? 0 : 1;
						if (inTag == 0) {
							buffer[bufferPos] = 0;
							addHTMLToList(buffer, bufferPos);
						} else {
							bufferPos = 0;
						}
					} else if (inTag) {
						buffer[bufferPos] = pageData[i];
						bufferPos++;
					}
				}

			} while (netbuf_next(inBuf) >= 0);

			// delete buffer
			if (inBuf != NULL)
				netbuf_delete(inBuf);

			// fetch next data
			inBuf = netconn_recv(conn);
		}

	} else {
		printf("WEBCLIENT: ERROR: %d\n", connErr);
	}

	// close connection
	if (conn != NULL) {
		while (netconn_delete(conn) != ERR_OK) {
			vTaskDelay(1);
		}
	}

	printf("WEBCLIENT: FINISHED\n");
	drawGWC();
}

void addHTMLToList(char* str, int len) {
	char buffer[32];
	char status = 0, *name, *id;
	int i, strLen;
	long value = 0;

	getElementType(str, buffer);
	if (strcmp(buffer, "NumberInputField") == 0) {
		status |= GWC_NUMERIC;
	} else if (strcmp(buffer, "CheckboxInputField") == 0) {
		status |= GWC_BOOLEAN;
	}

	if (status > 0) {
		if (getValueForParamName(str, "name", buffer) != NULL) {
			status |= GWC_LABEL;
			strLen = strlen(buffer);
			name = pvPortMalloc((strLen + 1) * sizeof(char));
			for (i = 0; i < strLen; i++) {
				name[i] = buffer[i];
			}
			name[i] = 0;
		}

		if (getValueForParamName(str, "value", buffer) != NULL) {
			value = atoi(buffer);
		}

		if (getValueForParamName(str, "id", buffer) != NULL) {
			strLen = strlen(buffer);
			id = pvPortMalloc((strLen + 1) * sizeof(char));
			for (i = 0; i < strLen; i++) {
				id[i] = buffer[i];
			}
			id[i] = 0;
			addRow(id, name == NULL ? "" : name, value, status);
		}
	}
}

char* getValueForParamName(char* str, char* search, char* retValue) {

	char *buffer;
	int i, j;
	buffer = strstr(str, search);
	if (buffer != NULL) {

		buffer = strstr(buffer, "=\"");
		buffer += 2;
		j = 0;
		for (i = 0; buffer[i] != '\"'; i++) {
			retValue[j] = buffer[i];
			j++;
		}
		retValue[j] = 0;
	}
	return retValue;
}

char *getElementType(char * str, char *buff) {
	int i = 0, pos = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n') {
		i++;
	}
	while (str[i] != ' ' && pos >= 0) {
		if (str[i] != ' ') {
			buff[pos] = str[i];
			pos++;
		}
		i++;
	}
	buff[pos] = 0;
	return buff;
}

void sendData(tWidget *pWidget) {
	char buffer[256];
	char valBuffer[100];
	// status variables
	u16_t length, bindErr, connErr, writeErr;
	u16_t port;

	strcpy(buffer, "GET /set.cgi?");

	gwcRow *akt = pgwcRoot;

	int i = 0;
	while (akt != NULL) {
		if (i > 0) {
			strcat(buffer, "&");
		}
		sprintf(valBuffer, "%s=%d", akt->id, akt->value);
		printf("%s\n", valBuffer);
		strcat(buffer, valBuffer);
		akt = akt->next;
		i++;
	}

	strcat(buffer, " HTTP/1.0\r\n\r\n");
	printf(buffer);

	// create new connection
	conn = netconn_new(NETCONN_TCP);

	// create a connection
	connErr = netconn_connect(conn, remoteIP, 80);
	if (conn != NULL && connErr == 0) {

		// send request
		writeErr = netconn_write(conn, &buffer, sizeof(buffer), NETCONN_NOCOPY);
		// Delete incoming data!
		while (inBuf != NULL) {
			do {
				// read data
				netbuf_data(inBuf, (void**) &pageData, &length);
			} while (netbuf_next(inBuf) >= 0);

			// delete buffer
			if (inBuf != NULL)
				netbuf_delete(inBuf);

			// fetch next data
			inBuf = netconn_recv(conn);
		}
		// end delete

	} else {
		printf("WEBCLIENT: ERROR: %d\n", connErr);
	}

	// close connection
	if (conn != NULL) {
		while (netconn_delete(conn) != ERR_OK) {
			vTaskDelay(1);
		}
	}
	printf("WEBCLIENT: SEND FINISHED\n");
}
