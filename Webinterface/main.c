/*
 * Copyright (c) 2008-2009 Open Software S/C Ltda
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 *
 * Author: Paulo da Silva (psilva@opensoftware-br.com)
 *
 */

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "hw_memmap.h"
#include "sysctl.h"
#include "ethernet.h"
#include "gpio.h"

#include "lwip/tcpip.h"
#include "lwiplib.h"
#include "lwip/netif.h"

#include "LWIPStack.h"
#include "ETHIsr.h"

static struct netif lwip_netif;
static unsigned long g_ulIPMode = IPADDR_USE_STATIC;

#define LINHA0		0
#define LINHA1		8
#define LINHA2		16
#define LINHA3		24
#define LINHA4		32
#define LINHA5		40
#define LINHA6		48
#define LINHA7		56

xQueueHandle xOLEDQueue;

void UART0StdioInit(unsigned long ulPortNum);
void uart1Init(void);
void myputs(const unsigned char *pucBuffer);
void UART1Send(const unsigned char *pucBuffer, unsigned long ulCount);

int ETHServiceTaskInit(const unsigned long ulPort);
int ETHServiceTaskFlush(const unsigned long ulPort, const unsigned long flCmd);
void LWIPServiceTaskInit(void *pvParameters);

extern int  __HEAP_START;

extern void *_sbrk(int incr)
{
    static unsigned char *heap = NULL;
    unsigned char *prev_heap;

    if (heap == NULL) {
        heap = (unsigned char *)&__HEAP_START;
    }
    prev_heap = heap;

    heap += incr;

    return (void *)prev_heap;
}

void lcd(void *pvParameters)
{
	for(;;)
	{
		vTaskDelay(500);
		RIT128x96x4StringDraw("Dado - 1", 0, LINHA5, 25);
		vTaskDelay(500);
		RIT128x96x4StringDraw("Dado - 2", 0, LINHA5, 25);
	}
}

void uart0Thread(void *pvParameters)
{
	UART0StdioInit(0);
	for(;;)
	{
		UART0printf("Test of uart0 9999\n\r");
		RIT128x96x4StringDraw("Uart0 - 10", 0, LINHA6, 25);
		vTaskDelay(1500);
		UART0printf("Test of uart0 1000\n\r");
		RIT128x96x4StringDraw("Uart0 - 12", 0, LINHA6, 25);
		vTaskDelay(1500);
	}
}

void uart1Thread(void *pvParameters)
{
	UART1StdioInit(1);
	for(;;)
	{
		UART1printf("Test of uart1 9999\n\r");
		RIT128x96x4StringDraw("Uart1 - 10", 0, LINHA7, 25);
		vTaskDelay(2500);
		UART1printf("Test of uart1 1000\n\r");
		RIT128x96x4StringDraw("Uart1 - 12", 0, LINHA7, 25);
		vTaskDelay(2500);
	}
}

void prvSetupHardware( void )
{
    /* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
    a workaround to allow the PLL to operate reliably. */
    if( DEVICE_IS_REVA2 )
    {
        SysCtlLDOSet( SYSCTL_LDO_2_75V );
    }

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet( SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ );

}

void ethernetThread(void *pvParameters)
{
	IP_CONFIG ipconfig;

	UART1printf("Aguardando 10segundos para iniciar Ethernet\n");
	vTaskDelay(10000);
	UART1printf("Iniciando a interface Ethernet\n");


	ETHServiceTaskInit(0);
	ETHServiceTaskFlush(0,ETH_FLUSH_RX | ETH_FLUSH_TX);

	ipconfig.IPMode = IPADDR_USE_STATIC;
	ipconfig.IPAddr=0xC0A80064;
	ipconfig.NetMask=0xFFFFFF00;
	ipconfig.GWAddr=0xC0A80001;

	LWIPServiceTaskInit((void *)&ipconfig);

	for(;;)
	{
	}
}

int main(int argc, char *argv[])
{
	prvSetupHardware();

	RIT128x96x4Init(1000000);
	RIT128x96x4StringDraw("FreeRTOS-5.1.1ok  ", 0, LINHA0, 15);
	RIT128x96x4StringDraw("LWIP comp00  1.3.0", 0, LINHA1, 15);
	RIT128x96x4StringDraw("Lwip Source from Wella   ", 0, LINHA2, 15);

	UART1StdioInit(1);

	xTaskCreate( lcd,"lcd", 1000, NULL, 3, NULL);
	xTaskCreate( uart0Thread,"uart0", 1000, NULL, 3, NULL);
	xTaskCreate( uart1Thread,"uart1", 1000, NULL, 3, NULL);
	xTaskCreate( ethernetThread,"ethernet", 1000, NULL, 3, NULL);

	vTaskStartScheduler();
	for(;;)
	{

	}
}

void vApplicationIdleHook(void)
{

}
void vApplicationTickHook(void)
{
}
