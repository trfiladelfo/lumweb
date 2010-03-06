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
#include "taglib/taglib.h" // Definition of Strings

#endif

#include "ethernet/httpd/cgi/ssiparams.h"


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

#endif // __IO_H__
