/*
 * debugUART.c
 *
 *  Created on: 06.10.2009
 *      Author: anzinger
 */

#include "debugUART.h"

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "gpio.h"
#include "sysctl.h"
#include "uart.h"
#include "string.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "GraphicsLibary/graphicObjects.h"

int init = 0;

//*****************************************************************************
//
// Initializes URAT0
//
//*****************************************************************************
void vInitDebug(void) {
	if (init == 0) {
		//
		// Enable the peripherals.
		//
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

		//
		// Enable processor interrupts.
		//
		//IntMasterEnable();

		//
		// Set GPIO A0 and A1 as UART pins.
		//
		GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

		//
		// Configure the UART for 115,200, 8-N-1 operation.
		//
		UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
				(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE
						| UART_CONFIG_PAR_NONE));

		//
		// Enable the UART interrupt.
		//
		//IntEnable(INT_UART0);
		//UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
		init = 1;
	}
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void vSendDebugUART(unsigned char *pucBuffer) {
	//
	// Loop while there are more characters to send.
	//
	int i;
	xGRAPHMessage xMessage;
	int len = sizeof(pucBuffer);

	xMessage.msg = pucBuffer;

	for (i = 0; i < len && pucBuffer[i] > 31 && pucBuffer[i] < 129; i++) {
		//
		// Write the next character to the UART.
		//
		UARTCharPut(UART0_BASE, pucBuffer[i]);
	}

	xQueueSend(xGRAPHQueue, &xMessage, (portTickType) 0);
}
