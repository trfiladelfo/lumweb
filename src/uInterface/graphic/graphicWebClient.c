/**
 * \addtogroup Graphic
 * @{
 *
 * \file graphicWebClient.c
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <string.h>
#include <stdlib.h>

#include "setup.h"

#include "hw_types.h"

#include "graphic/graphicWebClient.h"
#include "graphic/graphicClientBuilder.h"
#include "graphic/graphicLib.h"

#include "configuration/configloader.h"

#include "ethernet/LWIPStack.h"
#include "ethernet/httpd/cgi/io.h"

#include "lwip/netbuf.h"
#include "lwip/api.h"

#include "grlib/widget.h"

#include "utils.h"

void addHTMLToList(char* str, int len);

void vLoadPage(char *uri) {
	struct netconn *conn;
	struct netbuf *inBuf;
	char *pageData;

	char buffer[128];
	char inTag = 0;

	vDestroyClientEntities();

	vInitPanel();
	vShowBootText("loading new Page ...");

	// status variables
	u16_t length, connErr, writeErr;

	aktPage = 1;
	// Set the get String

	sprintf(buffer, "GET /%s HTTP/1.0\r\n\r\n", uri);
#ifdef DEBUG_GRAPHIC
	printf(buffer);
#endif

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
		bufferPos = 0;
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
#ifdef DEBUG_GRAPHIC
		printf("WEBCLIENT: ERROR: %d\n", connErr);
#endif
	}

	// close connection
	if (conn != NULL) {
		while (netconn_delete(conn) != ERR_OK) {
			vTaskDelay(1);
		}
	}
#ifdef DEBUG_GRAPHIC
	printf("WEBCLIENT: FINISHED\n");
#endif
	vDrawClientEntity();
}

void addHTMLToList(char* str, int len) {

	vTaskSuspendAll();
	{
		char buffer[32];
		char status = 0, *name, *id, *link;
		int value = 0, min = 0, max = 0, decimal = 0, increment = 1;

		getElementType(str, buffer);
		if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_NUMBERINPUTFIELD]) == 0) {
#ifdef DEBUG_GRAPHIC
			printf("NumericInputField found\n");
#endif
			status = GWC_NUMERIC;
		} else if (strcmp(buffer,
				g_pcConfigSSITags[SSI_INDEX_CHECKBOXINPUTFIELD]) == 0) {
#ifdef DEBUG_GRAPHIC
			printf("CheckboxInputField found\n");
#endif
			status = GWC_BOOLEAN;
		} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_SUBMITINPUTFIELD])
				== 0) {
#ifdef DEBUG_GRAPHIC
			printf("SubmitButton found\n");
#endif
			status = GWC_SUBMIT;
		} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_HYPERLINK]) == 0) {
#ifdef DEBUG_GRAPHIC
			printf("Hyperlink found\n");
#endif
			status = GWC_HYPERLINK;
		}

		if ((status & GWC_NUMERIC) == GWC_NUMERIC || (status & GWC_BOOLEAN)
				== GWC_BOOLEAN) {
			if (getValueForParamName(str, "name", buffer) != NULL) {
				status |= GWC_LABEL;
				name = pcStrdup(buffer);
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
				id = pcStrdup(buffer);
			}
#ifdef DEBUG_GRAPHIC
			printf("ADD NEW ENTITY: %s, %s, %d, %d, %d, %d, %d, 0x%X\n", id,name, value, min, max, increment, decimal, status);
#endif

			vAddRow(id, name == NULL ? "" : name, value, min, max, increment,
					decimal, status);

		} else if ((status & GWC_SUBMIT) == GWC_SUBMIT) {

			if (getValueForParamName(str, "label", buffer) != NULL) {
				name = pcStrdup(buffer);
			}
#ifdef DEBUG_GRAPHIC
			printf("new SubmitButton %s", name);
#endif
			vAddActionButton(name, NULL, status);

		} else if ((status & GWC_HYPERLINK) == GWC_HYPERLINK) {
			if (getValueForParamName(str, "name", buffer) != NULL) {
				name = pcStrdup(buffer);
			}
			if (getValueForParamName(str, "value", buffer) != NULL) {
				link = pcStrdup(buffer);
			}
#ifdef DEBUG_GRAPHIC
			printf(" \"%s\" - \"%s\"\n", name, link);
#endif
			vAddActionButton(name == NULL ? "" : name,
					link == NULL ? "" : link, status);
		}
	}
	xTaskResumeAll();
}

char* getValueForParamName(char* str, char* search, char* retValue) {

	vTaskSuspendAll();
	{
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
#ifdef DEBUG_GRAPHIC
		printf("getValueForParam: \"%s\" \"%s\"\n", search, retValue);
#endif
	}
	xTaskResumeAll();
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
	struct netconn *conn;
	struct netbuf *inBuf = NULL;
	char *pageData;
	char buffer[256];
	char valBuffer[100];
	// status variables
	u16_t length, connErr, writeErr;

	strcpy(buffer, "GET /set.cgi?");

	xClientEntity *akt = getClientRoot();

	int i = 0;
	while (akt != NULL) {
		if (akt->id != NULL) {
			if (i > 0) {
				strcat(buffer, "&");
			}
			sprintf(valBuffer, "%s=%d", akt->id, akt->value);
#ifdef DEBUG_GRAPHIC
			printf("%s\n", valBuffer);
#endif
			strcat(buffer, valBuffer);
			i++;
		}

		akt = akt->next;
	}

	strcat(buffer, " HTTP/1.0\r\n\r\n");
#ifdef DEBUG_GRAPHIC
	printf(buffer);
#endif

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
#ifdef DEBUG_GRAPHIC
		printf("WEBCLIENT: ERROR: %d\n", connErr);
#endif
	}

	// close connection
	if (conn != NULL) {
		while (netconn_delete(conn) != ERR_OK) {
			vTaskDelay(1);
		}
	}
#ifdef DEBUG_GRAPHIC
	printf("WEBCLIENT: SEND FINISHED\n");
#endif
}
