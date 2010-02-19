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

/**
 * \addtogroup CGIandSSI
 * @{
 *
 * \file io.c
 * \author Anziner, Hahn
 * \brief I/O routines for the enet_io example application.
 *
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

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

#include "ethernet/lwipopts.h"


/// Message for the Comm-Task
xComMessage xCom_msg;

#ifdef INCLUDE_HTTPD_CGI
//*****************************************************************************
//
//! Prototypes for the various CGI handler functions.
//
//*****************************************************************************
/**
 *
 * This CGI handler is called whenever the web browser requests set.cgi.
 * This CGI parses the GET Parameters and sets the values
 *
*/
static char *SetCGIHandler(int iIndex, int iNumParams, char *pcParam[],
		char *pcValue[]);

#endif

#ifdef INCLUDE_HTTPD_SSI
/*
 *
 * Prototype for the main handler used to process server-side-includes for the
 * application's web-based configuration screens.
 *
*/
#ifdef INCLUDE_HTTPD_SSI_PARAMS
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen,
		pSSIParam *params);
#else
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen );
#endif

#endif

/**
 * gets current date and time
 *
 * @param *pcBuf	pointer to char buffer for the date and time
 * @param iBufLen	length of buffer
 */
extern void get_dateandtime(char * pcBuf, int iBufLen);

#ifdef INCLUDE_HTTPD_CGI
//*****************************************************************************
//
// CGI URI indices for each entry in the g_psConfigCGIURIs array.
//
//*****************************************************************************
#define CGI_INDEX_CONTROL       0

/**
 *
 * This array is passed to the HTTPD server to inform it of special URIs
 * that are treated as common gateway interface (CGI) scripts.  Each URI name
 * is defined along with a pointer to the function which is to be called to
 * process it.
 *
 */
static const tCGI g_psConfigCGIURIs[] = { { "/set.cgi", SetCGIHandler }, // CGI_INDEX_CONTROL
		};

/**
 *
 * The number of individual CGI URIs that are configured for this system.
 *
 */
#define NUM_CONFIG_CGI_URIS     (sizeof(g_psConfigCGIURIs) / sizeof(tCGI))

//*****************************************************************************
//
//! The file sent back to the browser by default following completion of any
//! of our CGI handlers.  Each individual handler returns the URI of the page
//! to load in response to it being called.
//
//*****************************************************************************

#endif

/**
 *
 * Initialize IO and SSI Handlers
 *
 */
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

char **paramsSet = NULL, **valuesSet = NULL;
int paramValueLen; /// number of params/values set last time - 1

/**
 *
 * This CGI handler is called whenever the web browser requests set.cgi.
 * This CGI parses the GET Parameters and sets the values
 *
*/
static char *
SetCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
	int i;
	long value = 0, r_value, decimal_place = 0, hour = 0, minute = 0;
	char *name, save = 0, error = 0, *str_value = NULL, *str_decimal_place = NULL;

#if DEBUG_CGI
	printf("SetCGIHandler: new set.cgi request with %d Params\n", iNumParams);
#endif

	/*
	// TODO MEMORY HANDLING
	if (paramsSet != NULL && valuesSet != NULL) {
		for (i = 0; i <= paramValueLen; i++) {
			vPortFree(*(valuesSet + i));
			vPortFree(*(paramsSet + i));
		}

/		 vPortFree(**(paramsSet));
		 #if DEBUG_SSI
		 printf("io_print_saved_params: freed paramsSet \n");
		 #endif

		 vPortFree(**(valuesSet));
		 #if DEBUG_SSI
		 printf("io_print_saved_params: freed valuesSet \n");
		 #endif

	} else {
		// TODO statische allokkierung dynamisch machen
		paramsSet = pvPortMalloc(sizeof(char **) * 10);
		valuesSet = pvPortMalloc(sizeof(char **) * 10);
		paramValueLen = -1;
	}
*/
	//test if set was success full
	if (iNumParams > 0) {

		for (i = 0; i < iNumParams; i++) {
			name = pcParam[i];

			if (strcmp(name, "uid") == 0 || strcmp(name, "ajax") == 0) { // ignore params ajax and uid
				;
			} else {
				xCom_msg.cmd = SET;
				xCom_msg.dataSouce = DATA;
				xCom_msg.from = xHttpdQueue;
				xCom_msg.taskToResume = xLwipTaskHandle;
				xCom_msg.freeItem = pdFALSE;

				xCom_msg.item = name;
			/* check for checkbox value */
				if (strcmp(pcValue[i], "on") == 0) {
#if DEBUG_CGI
					printf("SetCGIHandler: Found Checkbox %s\n", name);
#endif
					xCom_msg.value = 1;
					save = 1;

				} else

			/*------ check for float value ----------------------*/
					if(name[0] == 'f' && name[1] == '_'){
						 // Found float value
						str_value = strtok(pcValue[i], ".");
						str_decimal_place = strtok(NULL, ".");

						if ((CheckDecimalParam((const char*) str_decimal_place, &decimal_place)
											== pdTRUE) && (CheckDecimalParam((const char*) str_value, &value)
											== pdTRUE)){

							name += 2; //remove 'f_'

							xCom_msg.value = value * 10 + decimal_place ; // zehntelschritte
							xCom_msg.item = name;
							save = 1;
#if DEBUG_CGI
							printf("SetCGIHandler: Found VALID float param: %s=%d.%d \n", name+2, (int)value, (int)decimal_place);
#endif
						}else{
#if DEBUG_CGI
							printf("SetCGIHandler: Found INVALID float param: %s=%s \n", name+2, pcValue[i]);
#endif
							save = 0;
							error = 1;
						}
				}else

			/*-----  check for time value ----------*/
					if(pcParam[i][0] == 't' && pcParam[i][1] == '_'){
							if (CheckDecimalParam((const char*) pcValue[i], &hour) == pdTRUE){
#if DEBUG_CGI
							printf("SetCGIHandler: Found first VALID time param - hour: %s=%d \n", pcParam[i]+2, (int)hour);
#endif
								//go to the next param , look for the minutes
								i++;
								if (i < iNumParams){
									if(pcParam[i][0] == 't' && pcParam[i][1] == '_'){

										if (CheckDecimalParam((const char*) pcValue[i] , &minute) == pdTRUE){
#if DEBUG_CGI
											printf("SetCGIHandler: Found second VALID time param - minute: %s=%d \n", pcParam[i]+2, (int)minute);
#endif
											name += 2; //remove t_

											xCom_msg.item = name;
											xCom_msg.value = hour * 60 + minute;
											save = 1;
											hour = 0;
											minute = 0;
										}else{
											printf("SetCGIHandler: Found second INVALID time param: %s=%s \n", pcParam[i]+2, pcValue[i]);

											error = 1;
										}
								}else{
#if DEBUG_CGI
									printf("SetCGIHandler: Found first INVALID time param: %s=%s \n", pcParam[i]+2, pcValue[i]);
#endif
									error = 1;
								}
							}
					}
				}else

			/*------ check for standard integer value ---------*/
					if (CheckDecimalParam((const char*) pcValue[i], &value)
						== pdTRUE) {

#if DEBUG_CGI
					printf("SetCGIHandler: Found integer param: %s=%d \n", name, (int)value);
#endif


					xCom_msg.value = value;
					save = 1;

				}

			/*---------- no valid param found !  -> ERROR --------*/
				else{
#if DEBUG_CGI
					printf("SetCGIHandler: ERROR invalid param %s=%s \n", name, pcValue[i]);
#endif
					error = 1;
				}

				if (save == 1) { // send value to comTask
					save = 0;
					xQueueSend(xComQueue, &xCom_msg, (portTickType) 0);


					/* Add params to global fields  for ssi tag SavedParams */
					/*
					if (paramsSet != NULL && valuesSet != NULL && i < 10) {
						*(paramsSet + i) = pvPortMalloc(strlen(name) + 1);
						*(valuesSet + i) = pvPortMalloc(strlen(pcValue[i]) + 1);

						if (*(paramsSet + i) != NULL && *(valuesSet + i)
								!= NULL) {
							strcpy(*(paramsSet + i), name);
							strcpy(*(valuesSet + i), pcValue[i]);
#if DEBUG_CGI
							printf("SetCGIHandler: added %s=%s to param/valueSet \n", *(paramsSet+i), *(valuesSet+i));
#endif
						} */
					paramValueLen = i;



					// gesetzten parameter reuecklesen, zur ueberpruefung ob ok
					r_value = io_get_value_from_comtask(name);

					if(r_value != value)
						error = 1;
				}

			}
		}
		if (FindCGIParameter("ajax", pcParam, iNumParams) == -1)
			return "/set_ok.ssi";
		else
			return "/set_oka.ssi";
	} else {
		error = 1;
	}

	if(error == 1)
		if (FindCGIParameter("ajax", pcParam, iNumParams) == -1)
			return "/set_ok.ssi";
		else
			return "/set_oka.ssi";
	else
			return "/set_nok.ssi";
}

#endif

#ifdef INCLUDE_HTTPD_SSI

/**
 * This function is called by the HTTP server whenever it encounters an SSI
 * tag in a web page.  The iIndex parameter provides the index of the tag in
 * the g_pcConfigSSITags array. This function writes the substitution text
 * into the pcInsert array, writing no more than iInsertLen characters.
 * max size of array: MAX_TAG_INSERT_LEN
*/
#ifdef INCLUDE_HTTPD_SSI_PARAMS
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen,
		pSSIParam *params)
#else
static int
SSIHandler(int iIndex, char *pcInsert, int iInsertLen )
#endif
{
#ifndef INCLUDE_HTTPD_SSI_PARAMS
	pSSIParam *params = NULL;
#endif

#if DEBUG_SSI
	printf("SSI HANDLER \n");
#endif
	//
	// Which SSI tag have we been passed?
	//
	switch (iIndex) {
	case SSI_INDEX_DATEANDTIME:
		get_dateandtime(pcInsert, iInsertLen);
		break;

	case SSI_INDEX_INTEGERINPUTFIELD:
		io_get_number_input_field(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_SUBMITINPUTFIELD:
		io_get_submit_input_button(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_SAVEDPARAMS:
		io_print_saved_params(pcInsert, iInsertLen);
		break;

	case SSI_INDEX_CHECKBOXINPUTFIELD:
		io_get_checkbox_input_field(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_HYPERLINK:
		io_get_hyperlink(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_TITEL:
		io_get_titel(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_GROUP:
		io_get_group(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_FLOATINPUTFIELD:
		io_get_float_input_field(pcInsert, iInsertLen, params);
		break;

	case SSI_INDEX_TIMEINPUTFIELD:
		io_get_time_input_field(pcInsert, iInsertLen, params);
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

/**
 *
 * Removes trailing blanks
 *
 * @param *pszStr	pointer to the string
 *
 * @return returns a pointer to the trimmed string
 *
 */

char* strtrimr(char *pszStr) {
	int i, j; /* Local counters */

	/*-------------------------------------------------*/

	j = i = strlen(pszStr) - 1; /* Calculate the length of the string */

	while (iIsSpace(pszStr[i]) && (i >= 0))

		/* WHILE string ends with a blank */
		/*1994-01-08/Bac Even if all chars are blanks (= 0) */

		pszStr[i--] = '\0'; /*- Replace blank with '\0' */

	return pszStr; /* Return no of replacements */
}

/**
 *
 * Removes leading blanks
 *
 * @param *pszStr	pointer to the string
 *
 * @return returns a pointer to the trimmed string
 *
 */
char* strtriml(char *pszStr) {
	int i = 0, j; /* Local counters */

	/*-------------------------------------------------*/

	j = strlen(pszStr) - 1; /* Calculate the length of the string */

	while (iIsSpace(pszStr[i]) && (i <= j))

		/* WHILE string starts with a blank */

		i++; /*- Count no of leading blanks */

	if (0 < i) /* IF leading blanks are found */
		strcpy(pszStr, &pszStr[i]); /*- Shift string to the left */

	return pszStr; /* Return no of replacements */
}

/**
 *
 * Removes trailing and leading blanks
 *
 * @param *pszStr	pointer to the string
 *
 * @return returns a pointer to the trimmed string
 *
 */
char* strtrim(char *pszStr) {
	char *ret;

	/*-------------------------------------------------*/

	ret = strtrimr(pszStr); /* Remove trailing blanks */
	ret = strtriml(ret); /* Remove leading blanks */

	return ret;
}
/**
 *
 * gets a value for $id from comTask
 *
 */
int io_get_value_from_comtask(char* id) {

#if DEBUG_SSI
	printf("io_get_value_from_comtask: getting values \n");
#endif
	xCom_msg.cmd = GET;
	xCom_msg.dataSouce = DATA;
	xCom_msg.from = xHttpdQueue;
	xCom_msg.taskToResume = xLwipTaskHandle;
	xCom_msg.freeItem = pdFALSE;

	xCom_msg.item = id;
	xQueueSend(xComQueue, &xCom_msg, (portTickType) 0);
#if DEBUG_SSI
	printf("io_get_value_from_comtask: sending req to com task \n");
#endif
	vTaskSuspend(xLwipTaskHandle);
#if DEBUG_SSI
	printf("io_get_value_from_comtask: suspend lwipTask \n");
#endif

	if (xQueueReceive(xHttpdQueue, &xCom_msg, ( portTickType ) 10 ) == pdTRUE) {
#if DEBUG_SSI
		printf("io_get_value_from_comtask: got values %s=%d \n", id,
				xCom_msg.value);
#endif
		return xCom_msg.value;
	} else
		return -1;
}
/**
 *
 * creates number input field  and +/- buttons
 *
 */
void io_get_number_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1;
	char *id = NULL, *label = NULL, *max = NULL, *min = NULL, *decimal = NULL, *increment = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");
	max = SSIParamGetValue(*(params), "max");
	min = SSIParamGetValue(*(params), "min");
	decimal = SSIParamGetValue(*(params), "decimal");
	increment = SSIParamGetValue(*(params), "increment");

	SSIParamDeleteAll(params);

	if (id != NULL && label != NULL) {
		if (min == NULL)
			min = "-1";
		if (max == NULL)
			max = "-1";
		if (decimal == NULL)
			decimal = "0";
		if (increment == NULL)
			increment = "1";

		value = io_get_value_from_comtask(id);
		if (value != -1) {
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ IntegerInputField name=\"%s\" value=\"%d\" id=\"%s\" max=\"%s\" min=\"%s\" decimal=\"%s\" increment=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi\" name=\"%s\" value=\"%d\" id=\"%s\" />"
						"<script>addB('%s',%s,%s,%s,%s);</script>", label, value, id,
					max, min, decimal, increment, label, id, value, id, id, max, min, increment, decimal);

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
/**
 *
 * creates submit input button
 *
 */
void io_get_submit_input_button(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL) {
		//if(ajax_id != NULL) // AJAX
		snprintf(pcBuf, iBufLen, "<!-- $ SubmitInputField label=\"%s\" $ -->"
			"<input type=\"submit\" value=\"%s\"/>", label, label);
		//else
		//	snprintf(pcBuf, iBufLen, "<!-- $ SubmitInputField label=%s $ -->"
		//			"<input type=\"submit\" name=\"%s\" value=\"%s\" />", label, label, label);
	} else {
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
}

/**
 *
 * prints the last set values/params
 *
*/
void io_print_saved_params(char * pcBuf, int iBufLen) {
	int i;
	if (paramValueLen == -1) {
		snprintf(pcBuf, iBufLen, "Keine Parameter gesetzt");
	} else {
	/*	for (i = 0; i <= paramValueLen; i++) {
#if DEBUG_SSI
			printf("io_print_saved_params: valueSet=%s, paramSet=%s \n",
					*(valuesSet + i), *(paramsSet + i));
#endif
		}
*/
		snprintf(pcBuf, iBufLen, "%d Parameter gesetzt", paramValueLen + 1);
	}
}

//*****************************************************************************
//
//
//
//*****************************************************************************
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

//*****************************************************************************
//
//
//
//*****************************************************************************
void io_get_hyperlink(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *value = NULL, *label = NULL;

	label = SSIParamGetValue(*(params), "label");
	value = SSIParamGetValue(*(params), "value");

	SSIParamDeleteAll(params);

	if (label != NULL && value != NULL) {
		snprintf(
				pcBuf,
				iBufLen,
				"<!-- $ Hyperlink name=\"%s\" value=\"%s\" $ --> <a href=\"%s\">%s</a>",
				label, value, value, label);
#if DEBUG_SSI
		printf("io_get_hyperlink: done \n");
#endif
	} else {
#if DEBUG_SSI
		printf("io_get_hyperlink: error no id and/or value found\n");
#endif
		snprintf(pcBuf, iBufLen,
				"Hyperlink: ERROR - error no id and/or value found");
	}
}

/**
 *
 * creates a titel line
 *
 */
void io_get_titel(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL) {
		snprintf(pcBuf, iBufLen, "<!-- $ Titel label=\"%s\" $ -->"
			"<h1>%s</h1>", label, label);
	} else {
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
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

	if (id != NULL && label != NULL) {
		value = io_get_value_from_comtask(id);

		// parse value (in minutes)
		hour = value / 60;
		minute = value - (hour*60);

		// TODO Add buttons
		if (value != -1) {
			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ TimeInputField name=\"%s\" value=\"%d\" id=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi\" name=\"t_%s\" value=\"%d\" id=\"%s_1\" />"
						":<input type=\"text\" class=\"fi\" name=\"t_%s\" value=\"%d\" id=\"%s_2\" />",
						label, value, id,
						label, id, hour, id, id, minute, id);

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

/**
 *
 * creates a float input field
 *
 */
void io_get_float_input_field(char * pcBuf, int iBufLen, pSSIParam *params) {
	int value = 1, decimal_place = 0;
	char *id = NULL, *label = NULL, *max = NULL, *min = NULL, *increment = NULL;

	label = SSIParamGetValue(*(params), "label");
	id = SSIParamGetValue(*(params), "id");
	max = SSIParamGetValue(*(params), "max");
	min = SSIParamGetValue(*(params), "min");
	increment = SSIParamGetValue(*(params), "increment");

	SSIParamDeleteAll(params);

	if (id != NULL && label != NULL) {
		if (min == NULL)
			min = "null";
		if (max == NULL)
			max = "null";
		if (increment == NULL)
			increment = "null";

		value = io_get_value_from_comtask(id);
		if (value != -1) {
			decimal_place = value % 10;
			value = value / 10;

			snprintf(
					pcBuf,
					iBufLen,
					"<!-- $ FloatInputField name=\"%s\" value=\"%d.%d\" id=\"%s\" max=\"%s\" min=\"%s\" increment=\"%s\" $ -->"
						"%s <input type=\"text\" class=\"fi\" name=\"f_%s\" value=\"%d.%d\" id=\"%s\" />"
						"<script>addB('%s',%s,%s,%s);</script>", label, value, decimal_place, id,
					max, min, increment, label, id, value, decimal_place, id, id, max, min, increment);

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

/**
 *
 * creates a group line
 *
 */
void io_get_group(char * pcBuf, int iBufLen, pSSIParam *params) {
	char *label = NULL;
	label = SSIParamGetValue(*(params), "label");

	SSIParamDeleteAll(params);

	if (label != NULL) {
		snprintf(pcBuf, iBufLen, "<!-- $ Group label=\"%s\" $ -->"
			"<h3>%s</h3>", label, label);
	} else {
		snprintf(pcBuf, iBufLen,
				"SubmitInputField: ERROR - no param label found ");
	}
}

/**
 adds a new element to the list

 @param *root 		pointer to root element of list
 @param *nameValue	pointer to name-value string ($name=$value)

 @return 0	element not added
*/
int SSIParamAdd(pSSIParam *root, char *nameValue) {
	char *value;
	pSSIParam nParam, tmp = *(root);

#if DEBUG_SSI_PARAMS
	printf("SSIParamAdd: %s \n", nameValue);
#endif

	value = strstr(nameValue, "=");
	value++;

	nParam = pvPortMalloc(sizeof(SSIParam));
	nParam->name = pvPortMalloc(strlen(nameValue) - strlen(value) + 1);
	nParam->value = pvPortMalloc(strlen(value) + 1);

	if (nParam != NULL && nParam->name != NULL && nParam->value != NULL) {

		snprintf(nParam->name, (strlen(nameValue) - strlen(value)), "%s",
				nameValue);
		sprintf(nParam->value, "%s", value);

		nParam->name = strtrim(nParam->name);
		nParam->value = strtrim(nParam->value);

#if DEBUG_SSI
		printf("Werte getrimmt\n");
#endif

		if (strlen(nParam->name) > 0) {

			nParam->next = tmp;
			*(root) = nParam;
#if DEBUG_SSI_PARAMS
			printf("SSIParamAdd: added element name: '%s' value: '%s' \n",
					nParam->name, nParam->value);
#endif
		} else {

#if DEBUG_SSI_PARAMS
			printf("SSIParamAdd: didnt insert element, name empty\n");
#endif
		}

	} else {
#if DEBUG_SSI_PARAMS
		printf(" ... fail\n");
#endif
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
	return 0;
}
/**
 gets an element with $name from the list

 @param  root	root element of list
 @param  *name	pointer to name of the element

 @return element with $name .... element found
 @return NULL .... element not found
*/
pSSIParam SSIParamGet(pSSIParam root, char *name) {
	pSSIParam ret = NULL;

	while (root != NULL) {
#if DEBUG_SSI_PARAMS
		printf("SSIParamGet: element name: '%s' \n", root->name);
#endif
		if (strcmp(root->name, name) == 0)
			return root;
		root = root->next;
	}

	return ret;
}
/**
 gets a value of an element with $name from the list

 @param root	root element of list
 @param *name	pointer to name of the element

 @return string with the value .... element found
 @return NULL .... element not found
*/
char* SSIParamGetValue(pSSIParam root, char *name) {
	pSSIParam p;
	char* value = NULL;

	p = SSIParamGet(root, name);
	if (p != NULL) {
		value = p->value;
#if DEBUG_SSI_PARAMS
		printf("SSIParamGetValue: found value '%s' for %s \n", value, name);
#endif
	}

	return value;
}
/**
 * deletes and frees all elements ot the list
 *
 * @param *root	pointer to root element of list
 *
*/
void SSIParamDeleteAll(pSSIParam *root) {
	pSSIParam p = (*root), del = NULL;

	while (p != NULL) {
#if DEBUG_SSI_PARAMS
		printf("SSIParamDeleteAll: delete element : %s \n", p->name);
#endif
		del = p;
		p = p->next;
		vPortFree(del->name);

#if DEBUG_SSI_PARAMS
		printf("SSIParamDeleteAll: freed name \n");
#endif

		vPortFree(del->value);

#if DEBUG_SSI_PARAMS
		printf("SSIParamDeleteAll: freed value \n");
#endif

		vPortFree(del);
	}

#if DEBUG_SSI_PARAMS
	printf("SSIParamDeleteAll: deleted all elements \n");
#endif
}
