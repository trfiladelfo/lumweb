//*****************************************************************************
//
// io.h - Prototypes for I/O routines
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
 * \file io.h
 * \author Anziner, Hahn
 * \brief Prototypes for I/O routines
 *
*/



#ifndef __IO_H__
#define __IO_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "ethernet/lwipopts.h"
#include "setup.h"

#ifdef INCLUDE_HTTPD_SSI


/**
 The number of individual SSI tags that the HTTPD server can expect to
 find in our configuration pages.
*/

#define NUM_CONFIG_SSI_TAGS     (sizeof(g_pcConfigSSITags) / sizeof (char *))

/**
 This array holds all the strings that are to be recognized as SSI tag
 names by the HTTPD server.  The server will call SSIHandler to request a
 replacement string whenever the pattern <!--#tagname--> (where tagname
 appears in the following array) is found in ".ssi", ".shtml" or ".shtm"
 files that it serves. Max size is MAX_TAG_NAME_LEN
*/
static const char * const g_pcConfigSSITags[] = { "DateTime", /// SSI_INDEX_DATEANDTIME
		"IntegerInputField", /// SSI_INDEX_INTEGERINPUTFIELD
		"SubmitInputField", /// SSI_INDEX_SUBMITINPUTFIELD
		"SavedParams",		///SSI_INDEX_SAVEDPARAMS
		"CheckboxInputField", /// SSI_INDEX_CHECKBOXINPUTFIELD
		"Hyperlink", /// SSI_INDEX_HYPERLINK
		"Titel", /// SSI_INDEX_TITLE
		"Group", /// SSI_INDEX_GROUP
		"TimeInputField", /// SSI_INDEX_TIMEINPUTFIELD
		"FloatInputField" /// SSI_INDEX_FLOATINPUTFIELD

};
/**
 * SSI tag indices for each entry in the g_pcSSITags array.
 */
#define SSI_INDEX_DATEANDTIME  			(0)
#define SSI_INDEX_INTEGERINPUTFIELD    	(1)
#define SSI_INDEX_SUBMITINPUTFIELD     	(2)
#define SSI_INDEX_SAVEDPARAMS    		(3)
#define SSI_INDEX_CHECKBOXINPUTFIELD    (4)
#define SSI_INDEX_HYPERLINK			    (5)
#define SSI_INDEX_TITEL				    (6)
#define SSI_INDEX_GROUP				    (7)
#define SSI_INDEX_TIMEINPUTFIELD	    (8)
#define SSI_INDEX_FLOATINPUTFIELD	    (9)

#endif

/** represents an SSI Parameter */
typedef struct SSIParam {
	char *name;
	char *value;
	struct SSIParam* next;
} SSIParam;

typedef SSIParam * pSSIParam;

void io_init(void);
void io_get_number_input_field(char * pcBuf, int iBufLen, pSSIParam *params);
void io_get_submit_input_button(char * pcBuf, int iBufLen, pSSIParam *params);
void io_print_saved_params(char * pcBuf, int iBufLen);
void io_get_checkbox_input_field(char * pcBuf, int iBufLen, pSSIParam *params);
void io_get_hyperlink(char * pcBuf, int iBufLen, pSSIParam *params);
void io_get_titel(char * pcBuf, int iBufLen, pSSIParam *params);
void io_get_group(char * pcBuf, int iBufLen, pSSIParam *params);
void io_get_time_input_field(char * pcBuf, int iBufLen, pSSIParam *params);
void io_get_float_input_field(char * pcBuf, int iBufLen, pSSIParam *params);

int io_get_value_from_comtask(char* id);

#ifdef __cplusplus
}
#endif

///  gets an element with $name from the list
int SSIParamAdd(pSSIParam* root, char* nameValue);

///  gets an element with $name from the list
pSSIParam SSIParamGet(pSSIParam root, char* name);

/// deletes and frees all elements ot the list
void SSIParamDeleteAll(pSSIParam* root);

/// gets a value of an element with $name from the list
char* SSIParamGetValue(pSSIParam root, char* name);

#endif // __IO_H__
