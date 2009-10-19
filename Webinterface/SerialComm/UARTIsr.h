//*****************************************************************************
//
// UARTlsr.h - Prototypes for the UART0 Driver.
//
//*****************************************************************************

#ifndef UARTISR_H_
#define UARTISR_H_

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
//! The number of serial to ethernet ports supported by this module.
//
//*****************************************************************************
#define MAX_UART_PORTS           2

//*****************************************************************************
//
//! The size of the ring buffers (RX).
//
//*****************************************************************************
#define RX_RING_BUF_SIZE        (128)

//*****************************************************************************
//
//! The size of the ring buffers (TX).
//
//*****************************************************************************
#define TX_RING_BUF_SIZE        (2)

	
//*****************************************************************************
//
// UART states for opened device.
//
//*****************************************************************************	
#define UART_OPENED			0x00
#define UART_ERROR			0x01
	
//*****************************************************************************
//
// UART configure struct.
//
//*****************************************************************************	
typedef struct UARTConfig
{
	unsigned long baudRate;
	unsigned char wordLen;
	unsigned char stop;
	unsigned char parity;
}UART_CONFIG;
	
//*****************************************************************************
//
// UART configure struct.
//
//*****************************************************************************	
#define NOP_EVENT			0x00
#define RX_EVENT			0x01
#define TX_EVENT			0x02


//*****************************************************************************
//
// Values that can be passed to UARTConfigSetExpClk as the ulConfig parameter
// and returned by UARTConfigGetExpClk in the pulConfig parameter.
// Additionally, the UART_CONFIG_PAR_* subset can be passed to
// UARTParityModeSet as the ulParity parameter, and are returned by
// UARTParityModeGet.
//
//*****************************************************************************
#define UART_CONFIG_WLEN_8      0x00000060  // 8 bit data
#define UART_CONFIG_WLEN_7      0x00000040  // 7 bit data
#define UART_CONFIG_WLEN_6      0x00000020  // 6 bit data
#define UART_CONFIG_WLEN_5      0x00000000  // 5 bit data
#define UART_CONFIG_STOP_ONE    0x00000000  // One stop bit
#define UART_CONFIG_STOP_TWO    0x00000008  // Two stop bits
#define UART_CONFIG_PAR_NONE    0x00000000  // No parity
#define UART_CONFIG_PAR_EVEN    0x00000006  // Even parity
#define UART_CONFIG_PAR_ODD     0x00000002  // Odd parity
#define UART_CONFIG_PAR_ONE     0x00000086  // Parity bit is one
#define UART_CONFIG_PAR_ZERO    0x00000082  // Parity bit is zero

//*****************************************************************************
//
// Prototypes for the APIs.
//
//*****************************************************************************
extern void UART0ServiceTaskInit(void);
extern void UART1ServiceTaskInit(void);
extern int UARTServiceTaskOpen(unsigned long ulPort);
extern int UARTServiceTaskConfig(unsigned long ulPort, UART_CONFIG * config); 
extern int UARTServiceTaskRead(unsigned long ulPort, unsigned char * UARTdata, unsigned long UARTDataCount);
extern int UARTServiceTaskWrite(unsigned long ulPort, unsigned char * UARTdata, unsigned long UARTDataCount);
extern int UARTServiceTaskClose(unsigned long ulPort);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /*UARTISR_H_*/
