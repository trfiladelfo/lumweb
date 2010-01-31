/*
 * graphicWebClient.c
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#include <string.h>
#include "hw_types.h"

#include "graphic/graphicWebClient.h"
#include "graphic/graphicClientBuilder.h"

#include "configuration/configloader.h"

#include "ethernet/LWIPStack.h"
#include "ethernet/httpd/cgi/io.h"

#include "lwip/netbuf.h"
#include "lwip/api.h"

#include "grlib/widget.h"

#include "utils.h"

struct netconn *conn;
struct netbuf *inBuf;
char *pageData;

void addHTMLToList(char* str, int len);

void vLoadPage(char *uri) {
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
					if (pageData[i] == DELIMITOR_CHAR) {
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
	vDrawClientEntity();
}

void addHTMLToList(char* str, int len) {
	char buffer[32];
	char status = 0, *name, *id, *link;
	int i, strLen;
	int value = 0, min = 0, max = 0, decimal = 0, increment = 1;

	getElementType(str, buffer);
	if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_NUMBERINPUTFIELD]) == 0) {
		printf("\t\tNumericInputField found\n");
		status = GWC_NUMERIC;
	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_CHECKBOXINPUTFIELD])
			== 0) {
		printf("\t\tCheckboxInputField found\n");
		status = GWC_BOOLEAN;
	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_SUBMITINPUTFIELD])
			== 0) {
		printf("\t\tSubmitButton found\n");
		status = GWC_SUBMIT;
	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_HYPERLINK]) == 0) {
		printf("\t\tHyperlink found\n");
		status = GWC_HYPERLINK;
	}

	if ((status & GWC_NUMERIC) == GWC_NUMERIC || (status & GWC_BOOLEAN)
			== GWC_BOOLEAN) {
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

		if (getValueForParamName(str, "min", buffer) != NULL) {
			min = atoi(buffer);
			status |= GWC_VALUE_MIN;
		}

		if (getValueForParamName(str, "max", buffer) != NULL) {
			max = atoi(buffer);
			status |= GWC_VALUE_MAX;
		}
		if (getValueForParamName(str, "increment", buffer) != NULL) {
			increment = atoi(buffer);
		}
		if (getValueForParamName(str, "decimal", buffer) != NULL) {
			decimal = atoi(buffer);
		}

		if (getValueForParamName(str, "id", buffer) != NULL) {
			strLen = strlen(buffer);
			id = pvPortMalloc((strLen + 1) * sizeof(char));
			for (i = 0; i < strLen; i++) {
				id[i] = buffer[i];
			}
			id[i] = 0;
		}

		printf("ADD NEW ENTITY: %s, %s, %d, %d, %d, %d, 0x%X\n", id, name, value,
				min, max, increment, decimal, status);
		vAddRow(id, name == NULL ? "" : name, value, min, max, increment,
				decimal, status);

	} else if ((status & GWC_SUBMIT) == GWC_SUBMIT) {

		if (getValueForParamName(str, "value", buffer) != NULL) {
			strLen = strlen(buffer);
			link = pvPortMalloc((strLen + 1) * sizeof(char));
			for (i = 0; i < strLen; i++) {
				link[i] = buffer[i];
			}
			link[i] = 0;
		}
		printf("new SubmitButton %s", link);
		vAddActionButton(link, NULL, status);

	} else if ((status & GWC_HYPERLINK) == GWC_HYPERLINK) {
		printf("new Hyperlink");
		if (getValueForParamName(str, "name", buffer) != NULL) {
			strLen = strlen(buffer);
			name = pvPortMalloc((strLen + 1) * sizeof(char));
			for (i = 0; i < strLen; i++) {
				name[i] = buffer[i];
			}
			name[i] = 0;
		}
		if (getValueForParamName(str, "value", buffer) != NULL) {
			strLen = strlen(buffer);
			link = pvPortMalloc((strLen + 1) * sizeof(char));
			for (i = 0; i < strLen; i++) {
				link[i] = buffer[i];
			}
			link[i] = 0;
		}
		printf(" \"%s\" - \"%s\"\n", name, link);
		vAddActionButton(name == NULL ? "" : name, link == NULL ? "" : link,
				status);
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

void vSendData(tWidget *pWidget) {
	char buffer[256];
	char valBuffer[100];
	// status variables
	u16_t length, bindErr, connErr, writeErr;
	u16_t port;

	strcpy(buffer, "GET /set.cgi?");

	xClientEntity *akt = xClientRoot;

	int i = 0;
	while (akt != NULL) {
		if (akt->id != NULL) {
			if (i > 0) {
				strcat(buffer, "&");
			}
			sprintf(valBuffer, "%s=%d", akt->id, akt->value);
			printf("%s\n", valBuffer);
			strcat(buffer, valBuffer);
			i++;
		}

		akt = akt->next;
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
