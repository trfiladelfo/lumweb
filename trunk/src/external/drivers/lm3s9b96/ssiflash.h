//*****************************************************************************
//
// ssiflash.h - Header file for the Winbond Serial Flash driver for the
//              eac-lm3s9b90 boards.
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

#ifndef _SSIFLASH_H_
#define _SSIFLASH_H_

//*****************************************************************************
//
// Exported function prototypes.
//
//*****************************************************************************
extern tBoolean SSIFlashInit(void);
extern tBoolean SSIFlashIDGet(unsigned char *pucManufacturer,
                              unsigned char *pucDevice);
extern unsigned long SSIFlashChipSizeGet(void);
extern unsigned long SSIFlashSectorSizeGet(void);
extern unsigned long SSIFlashBlockSizeGet(void);
extern tBoolean SSIFlashIsBusy(void);
extern tBoolean SSIFlashSectorErase(unsigned long ulAddress, tBoolean bSync);
extern tBoolean SSIFlashBlockErase(unsigned long ulAddress, tBoolean bSync);
extern tBoolean SSIFlashChipErase(tBoolean bSync);
extern unsigned long SSIFlashRead(unsigned long ulAddress,
                                  unsigned long ulLength,
                                  unsigned char *pucDst);
extern unsigned long SSIFlashWrite(unsigned long ulAddress,
                                   unsigned long ulLength,
                                   unsigned char *pucSrc);

#endif // _SSIFLASH_H_
