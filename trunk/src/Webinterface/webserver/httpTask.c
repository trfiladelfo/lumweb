/*
 FreeRTOS.org V5.3.1 - Copyright (C) 2003-2009 Richard Barry.

 This file is part of the FreeRTOS.org distribution.

 FreeRTOS.org is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License (version 2) as published
 by the Free Software Foundation and modified by the FreeRTOS exception.
 **NOTE** The exception to the GPL is included to allow you to distribute a
 combined work that includes FreeRTOS.org without being obliged to provide
 the source code for any proprietary components.  Alternative commercial
 license and support terms are also available upon request.  See the
 licensing section of http://www.FreeRTOS.org for full details.

 FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along
 with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
 Temple Place, Suite 330, Boston, MA  02111-1307  USA.


 ***************************************************************************
 *                                                                         *
 * Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
 *                                                                         *
 * This is a concise, step by step, 'hands on' guide that describes both   *
 * general multitasking concepts and FreeRTOS specifics. It presents and   *
 * explains numerous examples that are written using the FreeRTOS API.     *
 * Full source code for all the examples is provided in an accompanying    *
 * .zip file.                                                              *
 *                                                                         *
 ***************************************************************************

 1 tab == 4 spaces!

 Please ensure to read the configuration and relevant port sections of the
 online documentation.

 http://www.FreeRTOS.org - Documentation, latest information, license and
 contact details.

 http://www.SafeRTOS.com - A version that is certified for use in safety
 critical systems.

 http://www.OpenRTOS.com - Commercial support, development, porting,
 licensing and training services.
 */
/* Standard includes. */
//#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* uip includes. */
#include "hw_types.h"

#include "uip.h"
#include "uip_arp.h"
#include "httpd.h"
#include "timer.h"
#include "clock-arch.h"
#include "hw_ethernet.h"
#include "ethernet.h"

/* Include Queue staff */
#include "ComTask/comTask.h"
#include "GraphicsLibrary/graphicObjects.h"

#include "UipBasic/uipBasics.h"
#include "webserver/httpTask.h"

/* DEBUG STUFF */
#include "DebugTask/debugTask.h"
/*-----------------------------------------------------------*/
//
// IP vergabe jetzt in der main.c
//

/* How long to wait before attempting to connect the MAC again. */
#define uipINIT_WAIT    100

/* Shortcut to the header within the Rx buffer. */
#define xHeader ((struct uip_eth_hdr *) &uip_buf[ 0 ])

/* Standard constant. */
#define uipTOTAL_FRAME_HEADER_SIZE	54

/*-----------------------------------------------------------*/
// Global Variable for the Value to Set
int value;
/*
 * Send the uIP buffer to the MAC.
 */
static void prvENET_Send(void);

/*
 * Setup the MAC address in the MAC itself, and in the uIP stack.
 */
static void prvSetMACAddress(void);

/*
 * Port functions required by the uIP stack.
 */
void clock_init(void);
clock_time_t clock_time(void);

/*-----------------------------------------------------------*/

/* The semaphore used by the ISR to wake the uIP task. */
extern xSemaphoreHandle xEMACSemaphore;

/*-----------------------------------------------------------*/

void clock_init(void)
{
	/* This is done when the scheduler starts. */
}
/*-----------------------------------------------------------*/

clock_time_t clock_time(void)
{
	return xTaskGetTickCount();
}

void vHttpdTask(void *pvParameters)
{

	portBASE_TYPE i;

	extern void ( vEMAC_ISR)(void);

	vInitUip();

	httpd_init();

	while (vInitEMAC() != pdPASS)
	{
		vTaskDelay(uipINIT_WAIT);
	}

	for (;;)
	{
		/* Is there received data ready to be processed? */
		uip_len = uiGetEMACRxData(uip_buf);

		if (uip_len > 0)
		{
			/* Standard uIP loop taken from the uIP manual. */

			if (xHeader->type == htons( UIP_ETHTYPE_IP ))
			{
				uip_arp_ipin();
				uip_input();

				/* If the above function invocation resulted in data that
				 should be sent out on the network, the global variable
				 uip_len is set to a value > 0. */
				if (uip_len > 0)
				{
					uip_arp_out();
					prvENET_Send();
				}
			}
			else if (xHeader->type == htons( UIP_ETHTYPE_ARP ))
			{
				uip_arp_arpin();

				/* If the above function invocation resulted in data that
				 should be sent out on the network, the global variable
				 uip_len is set to a value > 0. */
				if (uip_len > 0)
				{
					prvENET_Send();
				}
			}
		}
		else
		{
			if (timer_expired(&periodic_timer))
			{
				timer_reset(&periodic_timer);
				for (i = 0; i < UIP_CONNS; i++)
				{
					uip_periodic( i );

					/* If the above function invocation resulted in data that
					 should be sent out on the network, the global variable
					 uip_len is set to a value > 0. */
					if (uip_len > 0)
					{
						uip_arp_out();
						prvENET_Send();
					}
				}

				/* Call the ARP timer function every 10 seconds. */
				if (timer_expired(&arp_timer))
				{
					timer_reset(&arp_timer);
					uip_arp_timer();
				}
			}
			else
			{
				/* We did not receive a packet, and there was no periodic
				 processing to perform.  Block for a fixed period.  If a packet
				 is received during this period we will be woken by the ISR
				 giving us the Semaphore. */
				xSemaphoreTake( xEMACSemaphore, configTICK_RATE_HZ / 2 );
			}
		}
	}
}
/*-----------------------------------------------------------*/

static void prvENET_Send(void)
{
	vInitialiseSend();
	vIncrementTxLength(uip_len);
	vSendBufferToMAC();
}


/*-----------------------------------------------------------*/

void vApplicationProcessFormInput(portCHAR *pcInputString,
		portBASE_TYPE xInputLength)
{
	char c = 0, param[20], *arg, *buf, q = 1, z = 1;
	int i, x;
	xCOMMessage xCom_msg;

	/* initialise xcommessage */
	xCom_msg.cmd = SET;
	xCom_msg.from = xHTTPDQueue;
	xCom_msg.taskToResume = xHttpTaskHandler;
	xCom_msg.dataSouce = DATA;
	xCom_msg.freeItem = pdTRUE;


	arg = pvPortMalloc(30);
	buf = pvPortMalloc(30);


	/* Process the form input sent by forms of the served HTML. */

	for (i = 0; i < xInputLength && c == 0; i++)
	{
		if (pcInputString[i] == '?')
			c = 1;
	}

	if (c == 1)
	{
		while (q == 1)
		{
			/* get argument string */
			for (x = 0; i < xInputLength && pcInputString[i] != '='; i++)
			{
				arg[x] = pcInputString[i];
				x++;
			}
			i++;
			arg[x] = 0;

			/* get parameter value */
			for (x = 0; i < xInputLength && pcInputString[i] != '&'; i++)
			{
				param[x] = pcInputString[i];
				x++;
			}
			param[x] = 0;

			i++;
			// checks end of GET input string
			if (i > xInputLength)
			{
				q = 0;
			}

			/* send input to com task */
			xCom_msg.item = arg;
			value =  atoi(param);
			xCom_msg.value = value;

			xQueueSend(xCOMQueue, &xCom_msg, (portTickType) 0);
			vTaskSuspend(xHttpTaskHandler);
			z++;
		}

	}
}
