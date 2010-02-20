//*****************************************************************************
//
// touch.h - Prototypes for the touch screen driver.
//
// Copyright (c) 2008-2009 Luminary Micro, Inc.  All rights reserved.
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

#ifndef __TOUCH_H__
#define __TOUCH_H__

//*****************************************************************************
//
// The lowest ADC reading assumed to represent a press on the screen.  Readings
// below this indicate no press is taking place.
//
//*****************************************************************************
#define TOUCH_MIN 150

//*****************************************************************************
//
// Prototypes for the functions exported by the touch screen driver.
//
//*****************************************************************************
extern volatile short g_sTouchX;
extern volatile short g_sTouchY;
extern short g_sTouchMin;
extern void TouchScreenIntHandler(void);
extern void TouchScreenInit(void);
extern void TouchScreenCallbackSet(long (*pfnCallback)(unsigned long ulMessage,
                                                       long lX, long lY));

#endif // __TOUCH_H__
