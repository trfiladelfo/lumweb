//*****************************************************************************
//
// set_pinout.h - Functions related to pinout configuration.
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

#ifndef _SET_PINOUT_H_
#define _SET_PINOUT_H_

//*****************************************************************************
//
// An enum defining the various daughter boards that can be attached to an
// lm3s9b96 development board.
//
//*****************************************************************************
typedef enum
{
    DAUGHTER_NONE,
}
tDaughterBoard;

//*****************************************************************************
//
// A global variable indicating which of the possible daughter boards is
// currently connected to the lm3s9b96 development board.
//
//*****************************************************************************
extern tDaughterBoard g_eDaughterType;

//*****************************************************************************
//
// Public function prototypes.
//
//*****************************************************************************
extern void PinoutSet(void);

#endif // _SET_PINOUT_H_
