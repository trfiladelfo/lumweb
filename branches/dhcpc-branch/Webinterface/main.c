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

#include "ComTask/comTask.h"   /* include communication task header */


static struct netif lwip_netif;
static unsigned long g_ulIPMode = IPADDR_USE_STATIC;

/* Size of the stack allocated to the OLED task. */
#define mainGRAPHIC_OBJECTS_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 3 )

/* Task priorities. */
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )

xQueueHandle xOLEDQueue;

int ETHServiceTaskInit(const unsigned long ulPort);
int ETHServiceTaskFlush(const unsigned long ulPort, const unsigned long flCmd);
void LWIPServiceTaskInit(void *pvParameters);

extern int  __HEAP_START;

extern void vuGraphicObjectsTestTask(void *pvParameters);

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
	RIT128x96x4StringDraw("FreeRTOS-5.1.1ok  ", 0, 10, 15);

	xTaskCreate(vuGraphicObjectsTestTask, (signed portCHAR *) "graphicObjects",
			mainGRAPHIC_OBJECTS_STACK_SIZE + 50, NULL, mainCHECK_TASK_PRIORITY - 1,
			NULL);

	/* Start the Communication Task (vComTask) to interact with the machine */
	xTaskCreate(vComTask, (signed portCHAR *) "comTask",
			Com_TASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

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
