/**
 * \addtogroup Graphic
 * @{
 *
 * \file graphicWebClient.c
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <stdio.h>
#include <string.h>

#include "webClient.h"
#include "tags.h"

#include "FreeRTOS.h"

#include "utils.h"

#include "lwip/api.h"
#include "lwip/ip_addr.h"
#include "lwip/netbuf.h"

#include "graphic/gui/dislpayBasics.h"
#include "ethernet/httpd/cgi/io.h"

#define DELIMITOR_CHAR '$'

struct ip_addr *remoteIP = NULL;

void vParseParameter(char* html, u16_t len);

void vLoadWebPage(char* page, basicDisplayLine* paramsParameter) {

	basicDisplayLine *params = paramsParameter;
	struct netconn *conn;
	struct netbuf *inBuf;
	char *pageData;

	u16_t connErr, writeErr, length, i = 0, bufferpos = 0;

	char buffer[HTTPC_BUFFER_LEN];
	char valBuffer[8];
	tBoolean first = true, output = false;

	strcpy(buffer, "GET /");
	strcat(buffer, page);
	while (params != NULL) {
		if (first == true) {
			strcat(buffer, "?");
			first = false;
		} else {
			strcat(buffer, "&");
		}
		strcat(buffer, params->label);
		snprintf(valBuffer, 8, "=%d", params->value);
		strcat(buffer, valBuffer);
		params = params->next;
	}

	vClearDisplay();

	strcat(buffer, " HTTP1.0\r\n\r\n");

	if (remoteIP == NULL) {
		remoteIP = getAddresFromConfig("REMOTE_IP");
		if (remoteIP == NULL) {
			vShowBootText("RemoteIP is NULL!");
			return;
		}
		printf("RemoteIP: ");
		printip(remoteIP);
		printf("\n");
	}

	// create new connection
	conn = netconn_new(NETCONN_TCP);

	// create a connection
	connErr = netconn_connect(conn, remoteIP, 80);

	if (conn != NULL && connErr == 0) {

		// send request
		writeErr = netconn_write(conn, &buffer, sizeof(buffer), NETCONN_NOCOPY);

		if (writeErr == 0) {

			do {
				// fetch next data
				inBuf = netconn_recv(conn);
				do {
					vTaskSuspendAll();
					{
						// read data
						netbuf_data(inBuf, (void**) &pageData, &length);

						for (i = 0; i < length && bufferpos < HTTPC_BUFFER_LEN; i++) {
							if (pageData[i] == DELIMITOR_CHAR) {
								if (output == true) {
									output = false;
									//printf("\n");
									buffer[bufferpos] = 0;
									vParseParameter(buffer, bufferpos);
								} else {
									output = true;
									bufferpos = 0;
									continue;
								}
							}

							if (output == true) {
								//printf("%c", pageData[i]);
								buffer[bufferpos] = pageData[i];
								bufferpos++;
							}

						}

					}
					xTaskResumeAll();

				} while (netbuf_next(inBuf) >= 0);

				// delete buffer
				if (inBuf != NULL) {
					netbuf_delete(inBuf);
				}

			} while (inBuf != NULL);
		} else {
			snprintf(buffer, HTTPC_BUFFER_LEN, "WEBCLIENT: READ ERROR: %d\n",
					connErr);
			printf(buffer);
			vShowBootText(buffer);
		}

	} else {
		snprintf(buffer, HTTPC_BUFFER_LEN, "WEBCLIENT: ERROR: %d\n", connErr);
		printf(buffer);
		vShowBootText(buffer);
	}

	// close connection
	if (conn != NULL) {
		while (netconn_delete(conn) != ERR_OK) {
			vTaskDelay(1);
		}
	}

	printf("\n");
}


/**
 * Parse the Special Komments for the GUI
 */
void vParseParameter(char* html, u16_t len) {
	int i, bufferpos = 0, type;

	char* buffer = (char*) pvPortMalloc(len * sizeof(char));

	printf("vParseParameter\n");

	for (i = 0; i < len; i++) {
		if (!iIsSpace(html[i])) {
			break;
		}
	}

	for (; i < len && !iIsSpace(html[i]); i++) {
		buffer[bufferpos] = html[i];
		bufferpos++;
	}
	buffer[bufferpos] = 0;
	printf("vParseParameter: Found Type: %s\n", buffer);

	// Typ feststellen
	if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_INTEGERINPUTFIELD]) == 0) {
		type = SSI_INDEX_INTEGERINPUTFIELD;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_SUBMITINPUTFIELD])
			== 0) {
		type = SSI_INDEX_SUBMITINPUTFIELD;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_CHECKBOXINPUTFIELD])
			== 0) {
		type = SSI_INDEX_CHECKBOXINPUTFIELD;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_HYPERLINK]) == 0) {
		type = SSI_INDEX_HYPERLINK;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_TITEL]) == 0) {
		type = SSI_INDEX_TITEL;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_TIMEINPUTFIELD]) == 0) {
		type = SSI_INDEX_TIMEINPUTFIELD;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_GROUP]) == 0) {
		type = SSI_INDEX_GROUP;

	} else if (strcmp(buffer, g_pcConfigSSITags[SSI_INDEX_FLOATINPUTFIELD])
			== 0) {
		type = SSI_INDEX_FLOATINPUTFIELD;

	} else {
		type = -1;
	}

	// Typ verarbeiten
	switch (type) {
	case SSI_INDEX_CHECKBOXINPUTFIELD:
		printf("vParseParameter: Checkbox\n");
		vParseCheckboxInputField(html, len);
		break;
	case SSI_INDEX_FLOATINPUTFIELD:
		printf("vParseParameter: Floatinput\n");
		vParseFloatInputField(html, len);
		break;
	case SSI_INDEX_GROUP:
		printf("vParseParameter: Group\n");
		vParseGroup(html, len);
		break;
	case SSI_INDEX_HYPERLINK:
		printf("vParseParameter: Hyperlink\n");
		vParseHyperlink(html, len);
		break;
	case SSI_INDEX_INTEGERINPUTFIELD:
		printf("vParseParameter: Integer\n");
		vParseIntegerInputField(html, len);
		break;
	case SSI_INDEX_SUBMITINPUTFIELD:
		printf("vParseParameter: Submit\n");
		xDisplayRoot.save = true;
		break;
	case SSI_INDEX_TIMEINPUTFIELD:
		printf("vParseParameter: Timeinput\n");
		vParseTimeInputField(html, len);
		break;
	case SSI_INDEX_TITEL:
		printf("vParseParameter: Titel\n");
		vParseTitle(html, len);
		break;
	case -1:
	default:
		printf("vParseParameter: ignore Param %s", buffer);
		break;
	}

	vPortFree(buffer);
}
