//*****************************************************************************
//
// uartstdio.h - Prototypes for the UART console functions.
//
// Copyright (c) 2007-2008 Luminary Micro, Inc.  All rights reserved.
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
// This is part of revision 3618 of the Stellaris Firmware Development Package.
//
//*****************************************************************************

#ifndef __UARTSTDIO_H__
#define __UARTSTDIO_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// If built for buffered operation, the following labels define the sizes of
// the transmit and receive buffers respectively.
//
//*****************************************************************************
#ifdef UART_BUFFERED
#ifndef UART_RX_BUFFER_SIZE
#define UART_RX_BUFFER_SIZE     128
#endif
#ifndef UART_TX_BUFFER_SIZE
#define UART_TX_BUFFER_SIZE     1024
#endif
#endif

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void UART0StdioInit(unsigned long ulPort);
extern int  UART0gets(char *pcBuf, unsigned long ulLen);
extern unsigned char UART0getc(void);
extern void UART0printf(const char *pcString, ...);
extern int UART0write(const char *pcBuf, unsigned long ulLen);
extern void uart0Init(void);
#ifdef UART_BUFFERED
extern int UART0Peek(unsigned char ucChar);
extern void UART0FlushTx(tBoolean bDiscard);
extern void UART0FlushRx(void);
extern int UART0RxBytesAvail(void);
extern int UART0TxBytesFree(void);
extern void UART0EchoSet(tBoolean bEnable);
#endif


extern void UART1StdioInit(unsigned long ulPort);
extern int  UART2gets(char *pcBuf, unsigned long ulLen);
extern unsigned char UART1getc(void);
extern void UART1printf(const char *pcString, ...);
extern int UART1write(const char *pcBuf, unsigned long ulLen);
extern void uart1Init(void);
#ifdef UART_BUFFERED
extern int UART1Peek(unsigned char ucChar);
extern void UART1FlushTx(tBoolean bDiscard);
extern void UART1FlushRx(void);
extern int UART1RxBytesAvail(void);
extern int UART1TxBytesFree(void);
extern void UART1EchoSet(tBoolean bEnable);
#endif

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __UARTSTDIO_H__
