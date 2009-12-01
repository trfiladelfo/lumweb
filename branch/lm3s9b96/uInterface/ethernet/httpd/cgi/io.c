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


// Message for the Comm-Task
xComMessage xCom_msg;


#ifdef INCLUDE_HTTPD_CGI
//*****************************************************************************
//
//! Prototypes for the various CGI handler functions.
//
//*****************************************************************************
static char *ControlCGIHandler(int iIndex, int iNumParams, char *pcParam[],
                              char *pcValue[]);

static char *TestCGIHandler(int iIndex, int iNumParams, char *pcParam[],
                              char *pcValue[]);

#endif

#ifdef INCLUDE_HTTPD_SSI
//*****************************************************************************
//
//! Prototype for the main handler used to process server-side-includes for the
//! application's web-based configuration screens.
//
//*****************************************************************************
static int SSIHandler(int iIndex, char *pcInsert, int iInsertLen);
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
static const tCGI g_psConfigCGIURIs[] =
{
    { "/iocontrol.cgi", ControlCGIHandler },      // CGI_INDEX_CONTROL
	{ "/test.cgi", TestCGIHandler },      		  // CGI_INDEX_CONTROL
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
#define DEFAULT_CGI_RESPONSE    "/io_cgi.shtml"
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
static const char *g_pcConfigSSITags[] =
{
    "DateTime",      // SSI_INDEX_DATEANDTIME
    "NumberInputField",      // SSI_INDEX_NUMBERINPUTFIELD
    "SubmitInputField"      // SSI_INDEX_SUBMITINPUTFIELD

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
void
io_init(void)
{    
    //
    // Enable GPIO bank F to allow control of the LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Configure Port F0 for as an output for the status LED.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_0);

    //
    // Initialize LED to OFF (0)
    //
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);

#ifdef INCLUDE_HTTPD_SSI
		//
		// Pass our tag information to the HTTP server.
		//
		http_set_ssi_handler(SSIHandler, g_pcConfigSSITags,
		                     NUM_CONFIG_SSI_TAGS);
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
// This CGI handler is called whenever the web browser requests iocontrol.cgi.
//
//*****************************************************************************
static char *
ControlCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
 /*   long lLEDState;

    //
    // Get each of the expected parameters.
    //
    lLEDState = FindCGIParameter("LEDOn", pcParam, iNumParams);

    //
    // We got all the parameters and the values were within the expected ranges
    // so go ahead and make the changes.
    //
    io_set_led((lLEDState == -1) ? false : true);
*/
    //
    // Send back the default response page.
    //
    return(DEFAULT_CGI_RESPONSE);
}

static char *
TestCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    long lLEDState;
/*
    
    // Get each of the expected parameters.
    lLEDState = FindCGIParameter("LEDOn", pcParam, iNumParams);

	if 	(lLEDState == -1)  // "LEDOn" nicht gefunden
	      {io_set_led(false);
		   return("/ledistaus.html"); // Send back the response page.
		  }
	else	 			// "LEDOn" gefunden
	  	  {io_set_led(true);
		   return("/ledistein.html");  // Send back the response page.
		  }
*/
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
static int
SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
    //
    // Which SSI tag have we been passed?
    //
    switch(iIndex)
    {
        case SSI_INDEX_DATEANDTIME:
            get_dateandtime(pcInsert, iInsertLen);
            break;

        case SSI_INDEX_NUMBERINPUTFIELD:
        	io_get_number_input_field(pcInsert, iInsertLen);
            break;

        case SSI_INDEX_SUBMITINPUTFIELD:
        	io_get_submit_input_button(pcInsert, iInsertLen);
            break;

        default:
            snprintf(pcInsert, iInsertLen, "??");
            break;
    }

    //
    // Tell the server how many characters our insert string contains.
    //
    return(strlen(pcInsert));
}
#endif


//*****************************************************************************
//
// creates number input field  and +/- buttons
//
//*****************************************************************************
void
io_get_number_input_field(char * pcBuf, int iBufLen)
{
	int value = 1;
	char *arg = "day_hour";
	if (1){
		printf("io_get_number_input_field: getting values \n");
		xCom_msg.cmd = GET;
		xCom_msg.dataSouce = DATA;
		xCom_msg.from = xHttpdQueue;
		xCom_msg.taskToResume = xLwipTaskHandle;
		xCom_msg.freeItem = pdFALSE;

		xCom_msg.item = arg;
		xQueueSend(xComQueue, &xCom_msg, (portTickType) 0);
		printf("io_get_number_input_field: sending req to com task \n");

		vTaskSuspend(xLwipTaskHandle);
		printf("io_get_number_input_field: suspend lwipTask \n");


		if ((xQueueReceive(xCom_msg.from, &xCom_msg, ( portTickType ) 10 ))
				== pdTRUE){
			printf("io_get_number_input_field: got values \n");

			//value = xCom_msg.value;
			snprintf(
					pcBuf, iBufLen,
					"<input type=\"text\" name=\"%s\" value=\"%d\" id=\"%s\" />"
					"<br /><input type=\"button\" value=\"+\" onclick=\"increase('%s');\" />"
					"<input type=\"button\" value=\"-\" onclick=\"decrease('%s');\" />",
					arg, value, arg, arg,
					arg);
		}
	}else {
		printf("io_get_number_input_field: error \n");

		snprintf(pcBuf, iBufLen, "ERROR: NO DATA");
	}
}

//*****************************************************************************
//
// creates submit input button
//
//*****************************************************************************
void
io_get_submit_input_button(char * pcBuf, int iBufLen)
{
	char *arg = "Submit";

	snprintf(pcBuf, iBufLen,
			"<input type=\"submit\" name=\"%s\" value=\"%s"" />", arg, arg);
}

