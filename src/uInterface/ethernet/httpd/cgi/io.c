//*****************************************************************************
//
// io.c - I/O routines for the enet_io example application.
//
// Copyright (c) 2007-2009 Luminary Micro, Inc.  All rights reserved.
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 4053 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "gpio.h"
#include "sysctl.h"
#include "io.h"
#include "lwip/opt.h"
#include "../httpd.h"
#include "cgifuncs.h"

#include "communication/comTask.h"
#include "queueConfig.h"
#include "taskConfig.h"

#define INCLUDE_HTTPD_SSI_PARAMS

// Message for the Comm-Task
xComMessage xCom_msg;

#ifdef INCLUDE_HTTPD_CGI
//*****************************************************************************
//
//! Prototypes for the various CGI handler functions.
//
//*****************************************************************************
static char *SetCGIHandler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]);

#endif

#ifdef INCLUDE_HTTPD_SSI
//*****************************************************************************
//
//! Prototype for the main handler used to process server-side-includes for the
//! application's web-based configuration screens.
//
//*****************************************************************************
#ifdef INCLUDE_HTTPD_SSI_PARAMS
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen,
		pSSIParam *params);
#else
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen );
#endif

#endif

extern void get_dateandtime(char * pcBuf, int iBufLen);

#ifdef INCLUDE_HTTPD_CGI
//*****************************************************************************
//
// CGI URI indices for each entry in the g_psConfigCGIURIs array.
//
//*****************************************************************************
#define CGI_INDEX_CONTROL       0

//*****************************************************************************
//
//! This array is passed to the HTTPD server to inform it of special URIs
//! that are treated as common gateway interface (CGI) scripts.  Each URI name
//! is defined along with a pointer to the function which is to be called to
//! process it.
//
//*****************************************************************************
static const tCGI g_psConfigCGIURIs[] = { { "/set.cgi", SetCGIHandler }, // CGI_INDEX_CONTROL
		};

//*****************************************************************************
//
//! The number of individual CGI URIs that are configured for this system.
//
//*****************************************************************************
#define NUM_CONFIG_CGI_URIS     (sizeof(g_psConfigCGIURIs) / sizeof(tCGI))

//*****************************************************************************
//
//! The file sent back to the browser by default following completion of any
//! of our CGI handlers.  Each individual handler returns the URI of the page
//! to load in response to it being called.
//
//*****************************************************************************

#endif

#ifdef INCLUDE_HTTPD_SSI
//*****************************************************************************
//
// SSI tag indices for each entry in the g_pcSSITags array.
//
//*****************************************************************************
#define SSI_INDEX_DATEANDTIME  0
#define SSI_INDEX_NUMBERINPUTFIELD     1
#define SSI_INDEX_SUBMITINPUTFIELD     2

//*****************************************************************************
//
// This array holds all the strings that are to be recognized as SSI tag
// names by the HTTPD server.  The server will call SSIHandler to request a
// replacement string whenever the pattern <!--#tagname--> (where tagname
// appears in the following array) is found in ".ssi", ".shtml" or ".shtm"
// files that it serves. Max size is MAX_TAG_NAME_LEN
//
//*****************************************************************************
static const char *g_pcConfigSSITags[] = { "DateTime", // SSI_INDEX_DATEANDTIME
		"NumberInputField", // SSI_INDEX_NUMBERINPUTFIELD
		"SubmitInputField" // SSI_INDEX_SUBMITINPUTFIELD

		};

//*****************************************************************************
//
//! The number of individual SSI tags that the HTTPD server can expect to
//! find in our configuration pages.
//
//*****************************************************************************
#define NUM_CONFIG_SSI_TAGS     (sizeof(g_pcConfigSSITags) / sizeof (char *))

#define JAVASCRIPT_HEADER                                                     \
    "<script type='text/javascript' language='JavaScript'><!--\n"
#define JAVASCRIPT_FOOTER                                                     \
    "//--></script>\n"
#endif

//*****************************************************************************
//
// Initialize the IO used in this demo
// 1. STATUS LED on Port F pin 0
//
//*****************************************************************************
void io_init(void) {

#ifdef INCLUDE_HTTPD_SSI
	//
	// Pass our tag information to the HTTP server.
	//
	http_set_ssi_handler(SSIHandler, g_pcConfigSSITags, NUM_CONFIG_SSI_TAGS);
#endif

#ifdef INCLUDE_HTTPD_CGI
	//
	// Pass our CGI handlers to the HTTP server.
	//
	http_set_cgi_handlers(g_psConfigCGIURIs, NUM_CONFIG_CGI_URIS);
#endif
}

#ifdef INCLUDE_HTTPD_CGI
//*****************************************************************************
//
// This CGI handler is called whenever the web browser requests set.cgi.
// This CGI parses the GET Parameters and sets the values
//
//*****************************************************************************
static char *
SetCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	printf("SetCGIHandler: %d Params\n", iNumParams);

	if (iNumParams > 0) { //test if set was success full
		return "/set_ok.htm";
	} else {
		return "/set_nok.htm";
	}
}

#endif

#ifdef INCLUDE_HTTPD_SSI

//*****************************************************************************
//
// This function is called by the HTTP server whenever it encounters an SSI
// tag in a web page.  The iIndex parameter provides the index of the tag in
// the g_pcConfigSSITags array. This function writes the substitution text
// into the pcInsert array, writing no more than iInsertLen characters.
// max size of array: MAX_TAG_INSERT_LEN
//
//*****************************************************************************
#ifdef INCLUDE_HTTPD_SSI_PARAMS
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen,
		pSSIParam *params)
#else
static int
SSIHandler(int iIndex, char *pcInsert, int iInsertLen )
#endif
{

	//
	// Which SSI tag have we been passed?
	//
	switch (iIndex) {
	case SSI_INDEX_DATEANDTIME:
		get_dateandtime(pcInsert, iInsertLen);
		break;

	case SSI_INDEX_NUMBERINPUTFIELD:
		io_get_number_input_field(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_SUBMITINPUTFIELD:
		io_get_submit_input_button(pcInsert, iInsertLen, params);
		break;

	default:
		snprintf(pcInsert, iInsertLen, "??");
		break;
	}

	//
	// Tell the server how many characters our insert string contains.
	//
	return (strlen(pcInsert));
}
#endif

char* strtrimr(char *pszStr) {
	int i, j; /* Local counters */

	/*-------------------------------------------------*/

	j = i = strlen(pszStr) - 1; /* Calculate the length of the string */

	while (isspace(pszStr[i]) && (i >= 0))

		/* WHILE string ends with a blank */
		/*1994-01-08/Bac Even if all chars are blanks (= 0) */

		pszStr[i--] = '\0'; /*- Replace blank with '\0' */

	return pszStr; /* Return no of replacements */
}

char* strtriml(char *pszStr) {
	int i = 0, j; /* Local counters */

	/*-------------------------------------------------*/

	j = strlen(pszStr) - 1; /* Calculate the length of the string */

	while (isspace(pszStr[i]) && (i <= j))

		/* WHILE string starts with a blank */

		i++; /*- Count no of leading blanks */

	if (0 < i) /* IF leading blanks are found */
		strcpy(pszStr, &pszStr[i]); /*- Shift string to the left */

	return pszStr; /* Return no of replacements */
}

char* strtrim(char *pszStr) {
	char *ret;

	/*-------------------------------------------------*/

	ret = strtrimr(pszStr); /* Remove trailing blanks */
	ret = strtriml(ret); /* Remove leading blanks */

	return ret;
}
//*****************************************************************************
//
// creates number input field  and +/- buttons
//
//*****************************************************************************
void io_get_number_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1;
	pSSIParam p = NULL;
	char *id = "INVALID";
	xComMessage ret;

	p = SSIParamGet(*(params), "id");
	if (p != NULL)
		id = p->value;

	SSIParamDeleteAll(params);

	if (1) {
		printf("io_get_number_input_field: getting values \n");
		xCom_msg.cmd = GET;
		xCom_msg.dataSouce = DATA;
		xCom_msg.from = xHttpdQueue;
		xCom_msg.taskToResume = xLwipTaskHandle;
		xCom_msg.freeItem = pdFALSE;

		xCom_msg.item = id;

		xQueueSend(xComQueue, &xCom_msg, (portTickType) 0);
		printf("io_get_number_input_field: sending req to com task \n");

		vTaskSuspend(xLwipTaskHandle);
		printf("io_get_number_input_field: suspend lwipTask \n");

		if (xQueueReceive(xHttpdQueue, &ret, ( portTickType ) 0 ) == pdTRUE) {
			//		if ((xQueueReceive(xCom_msg.from, &xCom_msg, ( portTickType ) 10 ))
			//				== pdTRUE){
			//		if(1){
			printf("io_get_number_input_field: got values \n");

			value = ret.value;
			snprintf(
					pcBuf,
					iBufLen,
					"<input type=\"text\" name=\"%s\" value=\"%d\" id=\"%s\" />"
						"<br /><input type=\"button\" value=\"+\" onclick=\"increase('%s');\" />"
						"<input type=\"button\" value=\"-\" onclick=\"decrease('%s');\" />",
					id, value, id, id, id);
		} else {
			printf("io_get_number_input_field: error \n");
			snprintf(pcBuf, iBufLen, "ERROR: NO DATA");
		}
	}
}

//*****************************************************************************
//
// creates submit input button
//
//*****************************************************************************
void io_get_submit_input_button(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *arg = "Submit";

	snprintf(pcBuf, iBufLen, "<!-- SubmitInputField %s -->"
		"<input type=\"submit\" name=\"%s\" value=\"%s" " />", arg, arg, arg);
}

int SSIParamAdd(pSSIParam* root, char* nameValue) {
	int rc = 0;
	char *value, *name;
	int strnc;

	pSSIParam nParam, tmp = *(root);

	nParam = pvPortMalloc(sizeof(SSIParam));
	nParam->name = pvPortMalloc(strlen(name) + 1);
	nParam->value = pvPortMalloc(strlen(value) + 1);

	if (nParam != NULL && nParam->name != NULL && nParam->value != NULL) {
		value = strstr(nameValue, "=");
		value++;
		printf("Position von = geholt\n");
		snprintf(nParam->name, (strlen(nameValue) - strlen(value)), "%s", nameValue);
		sprintf(nParam->value, "%s", value);

		//ma nParam->name = strtrim(nParam->name);
		//ma nParam->value = strtrim(nParam->value);
		//ma printf("Werte getrimmt\n");

		if (strlen(nParam->name) > 0) {
			if (*(root) == NULL) {
				*(root) = nParam;
			} else {
				while (tmp->next != NULL) {
					tmp = tmp->next;
				}
				tmp->next = nParam;
			}
			printf("SSIParamAdd: added element name: '%s' value: '%s' \n",
					nParam->name, nParam->value);
		} else {
			printf("Nicht eingefuegt, da leer\n");
		}

	} else {
		printf(" ... fail\n");
		if (nParam->name != NULL) {
			vPortFree(nParam->name);
		}
		if (nParam->value != NULL) {
			vPortFree(nParam->value);
		}
		if (nParam != NULL) {
			vPortFree(nParam);
		}
	}
}

pSSIParam SSIParamGet(pSSIParam root, char* name) {
	pSSIParam ret = NULL;

	printf("SSIParamGet: element name: '%s' \n", root->name);
	while (root != NULL) {
		printf("SSIParamGet: element name: '%s' \n", root->name);

		if (strcmp(root->name, name) == 0)
			return root;
		root = root->next;
	}

	return ret;
}

void SSIParamDeleteAll(pSSIParam* root) {
	pSSIParam p = (*root), del = NULL;

	while (p != NULL) {
		del = p;
		p = p->next;
		vPortFree(del->name);
		vPortFree(del->value);
		vPortFree(del);
	}
}
