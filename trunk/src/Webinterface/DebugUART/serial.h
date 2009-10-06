#ifndef SERIAL_H__
#define SERIAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "typedef.h"
#include "FreeRtos.h"

void    SerialPortInit( int port, u32 Baudrate, u16 RxQueueLen, u16 TxQueueLen);
BOOL    SerialGetChar( u8* pChar, portTickType timeout);
u16     SerialGetArray( u8* pBuf, u16 len, portTickType timeout);
u16     SerialGetLine( u8* pBuf, u16 len, portTickType timeout);
u16 	SerialGetLineWithEcho( u8* pBuf, u16 len, portTickType timeout);
BOOL    SerialPutChar( u8 Char, portTickType timeout);
u16     SerialPutArray( u8* pBuf, u16 len, portTickType timeout);
void    SerialPrintf(char* pString, ...);
u16     SerialTxCharCount(void);
u16     SerialRxCharCount(void);

#ifdef __cplusplus
}
#endif


#endif //SERIAL_H__

