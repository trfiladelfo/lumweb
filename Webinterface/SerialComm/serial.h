#ifndef SERIAL_H__
#define SERIAL_H__

#include "typedef.h"
#include "FreeRTOS.h"

extern void    SerialPortInit( int port, u32 Baudrate, u16 RxQueueLen, u16 TxQueueLen);
extern BOOL    SerialGetChar( u8* pChar, portTickType timeout);
extern u16     SerialGetArray( u8* pBuf, u16 len, portTickType timeout);
extern u16     SerialGetLine( u8* pBuf, u16 len, portTickType timeout);
extern u16 	SerialGetLineWithEcho( u8* pBuf, u16 len, portTickType timeout);
extern BOOL    SerialPutChar( u8 Char, portTickType timeout);
extern u16     SerialPutArray( u8* pBuf, u16 len, portTickType timeout);
extern void    SerialPrintf(char* pString, ...);
extern u16     SerialTxCharCount(void);
extern u16     SerialRxCharCount(void);

#endif //SERIAL_H__

