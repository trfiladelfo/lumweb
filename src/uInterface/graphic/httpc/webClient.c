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

#include "graphic/gui/displayBasics.h"
#include "ethernet/httpd/cgi/io.h"

#include "setup.h"

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
		if (params->id != NULL) {
			if (first == true) {
				strcat(buffer, "?");
				first = false;
			} else {
				strcat(buffer, "&");
			}
			strcat(buffer, params->id);
			snprintf(valBuffer, 8, "=%d", params->value);
			strcat(buffer, valBuffer);
		}
		params = params->next;

	}
	strcat(buffer, " HTTP1.0\r\n\r\n");
#if DEBUG_HTTPC
	printf(buffer);
#endif

	vClearDisplay();

	if (remoteIP == NULL) {
		remoteIP = getAddresFromConfig("REMOTE_IP");
		if (remoteIP == NULL) {
			vShowBootText("RemoteIP is NULL!");
			return;
		}

#if DEBUG_HTTPC
		printf("RemoteIP: ");
		printip(remoteIP);
		printf("\n");
#endif
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
									buffer[bufferpos] = 0;
									vParseParameter(buffer, bufferpos);
								} else {
									output = true;
									bufferpos = 0;
									continue;
								}
							}

							if (output == true) {
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
#if DEBUG_HTTPC
	printf("\n");
#endif
}

/**
 * Parse the Special Komments for the GUI
 */
void vParseParameter(char* html, u16_t len) {
	int i, bufferpos = 0, type;

	char* buffer = (char*) pvPortMalloc(len * sizeof(char));

#if DEBUG_HTTPC
	printf("vParseParameter\n");
#endif

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

#if DEBUG_HTTPC
	printf("vParseParameter: Found Type: %s\n", buffer);
#endif

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
#if DEBUG_HTTPC
		printf("vParseParameter: Checkbox\n");
#endif
		vParseCheckboxInputField(html, len);
		break;
	case SSI_INDEX_FLOATINPUTFIELD:
#if DEBUG_HTTPC
		printf("vParseParameter: Floatinput\n");
#endif
		vParseFloatInputField(html, len);
		break;
	case SSI_INDEX_GROUP:
#if DEBUG_HTTPC
		printf("vParseParameter: Group\n");
#endif
		vParseGroup(html, len);
		break;
	case SSI_INDEX_HYPERLINK:
#if DEBUG_HTTPC
		printf("vParseParameter: Hyperlink\n");
#endif
		vParseHyperlink(html, len);
		break;
	case SSI_INDEX_INTEGERINPUTFIELD:
#if DEBUG_HTTPC
		printf("vParseParameter: Integer\n");
#endif
		vParseIntegerInputField(html, len);
		break;
	case SSI_INDEX_SUBMITINPUTFIELD:
#if DEBUG_HTTPC
		printf("vParseParameter: Submit\n");
#endif
		xDisplayRoot.save = true;
		break;
	case SSI_INDEX_TIMEINPUTFIELD:
#if DEBUG_HTTPC
		printf("vParseParameter: Timeinput\n");
#endif
		vParseTimeInputField(html, len);
		break;
	case SSI_INDEX_TITEL:
#if DEBUG_HTTPC
		printf("vParseParameter: Titel\n");
#endif
		vParseTitle(html, len);
		break;
	case -1:
	default:
#if DEBUG_HTTPC
		printf("vParseParameter: ignore Param %s", buffer);
#endif
		break;
	}

	vPortFree(buffer);
}
