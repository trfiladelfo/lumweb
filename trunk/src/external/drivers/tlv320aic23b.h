//*****************************************************************************
//
// tlv320aic23b.h - Prototypes and macros for the I2S controller.
//
// Copyright (c) 2009 Luminary Micro, Inc.  All rights reserved.
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
// This is part of revision 4905 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#ifndef __TLV320AIC23B_H__
#define __TLV320AIC23B_H__

//
// The values to use with the TLV320AIC23BLineInVolumeSet() function.
//
#define TLV_LINEIN_VC_MAX       0x1f
#define TLV_LINEIN_VC_MIN       0x00
#define TLV_LINEIN_VC_0DB       0x17
#define TLV_LINEIN_VC_MUTE      0x80

tBoolean TLV320AIC23BInit(void);
void TLV320AIC23BHeadPhoneVolumeSet(unsigned long ulVolume);
unsigned long TLV320AIC23BHeadPhoneVolumeGet(void);
void TLV320AIC23BLineInVolumeSet(unsigned char ucVolume);

#endif

