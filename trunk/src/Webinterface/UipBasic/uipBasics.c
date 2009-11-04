/*
 * uipBasics.c
 *
 *  Created on: 04.11.2009
 *      Author: root
 */

#include "uipBasics.h"

#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
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

#include "hw_memmap.h"

//#include "webserver/httpd.h"
#include "DebugTask/debugTask.h"

int initialized = 0;

/* The semaphore used by the ISR to wake the uIP task. */
extern xSemaphoreHandle xEMACSemaphore;

static void prvSetMACAddress(void);

/**
 * Initializes the UIP-Task without an IP Address
 */
void vInitUip(void)
{
	if (!initialized)
	{
		unsigned char buffer[100];
		unsigned portLONG ulUser0, ulUser1;
		unsigned char pucMACArray[8];
		portBASE_TYPE i;
		struct timer periodic_timer, arp_timer;

		/* Enable/Reset the Ethernet Controller */
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
		SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

		/* Create the semaphore used by the ISR to wake this task. */
		vSemaphoreCreateBinary(xEMACSemaphore);

		/* Initialise the uIP stack. */
		timer_set(&periodic_timer, configTICK_RATE_HZ / 2);
		timer_set(&arp_timer, configTICK_RATE_HZ * 10);

		uip_init();
		uip_arp_init();

		prvSetMACAddress();
	}
}

/**
 * Initializes the IP-Address
 */
void vUipSetIp(char ip1, char ip2, char ip3, char ip4)
{
	uip_ipaddr_t xIPAddr;

	ucIp1 = ip1;
	ucIp2 = ip2;
	ucIp3 = ip3;
	ucIp4 = ip4;

	uip_ipaddr(xIPAddr, ucIp1, ucIp2, ucIp3, ucIp4);
	uip_sethostaddr(xIPAddr);
}

/**
 * Initializes the UIP with an IP-Address
 */
void vInitUipWithIp(char ip1, char ip2, char ip3, char ip4)
{
	vInitUip();
	vUipSetIp(ip1, ip2, ip3, ip4);
}

/*-----------------------------------------------------------*/
/**
 * Private Function to get the MAC-Address
 */
static void prvSetMACAddress(void)
{
	unsigned portLONG ulUser0, ulUser1;
	unsigned char pucMACArray[8];
	struct uip_eth_addr xAddr;

	/* Get the device MAC address from flash */
	FlashUserGet(&ulUser0, &ulUser1);

	/* Convert the MAC address from flash into sequence of bytes. */
	pucMACArray[0] = ((ulUser0 >> 0) & 0xff);
	pucMACArray[1] = ((ulUser0 >> 8) & 0xff);
	pucMACArray[2] = ((ulUser0 >> 16) & 0xff);
	pucMACArray[3] = ((ulUser1 >> 0) & 0xff);
	pucMACArray[4] = ((ulUser1 >> 8) & 0xff);
	pucMACArray[5] = ((ulUser1 >> 16) & 0xff);

	/* Program the MAC address. */
	EthernetMACAddrSet(ETH_BASE, pucMACArray);

	xAddr.addr[0] = pucMACArray[0];
	xAddr.addr[1] = pucMACArray[1];
	xAddr.addr[2] = pucMACArray[2];
	xAddr.addr[3] = pucMACArray[3];
	xAddr.addr[4] = pucMACArray[4];
	xAddr.addr[5] = pucMACArray[5];
	uip_setethaddr( xAddr );
}

/**
 * Appcall if an Interrupts occurs
 */
void vUipAppcall(void)
{
	//if (uip_conn->lport == HTTP_PORT)
	//{
		httpd_appcall();
	//}
	//else if (uip_conn->lport == DEBUG_PORT)
	//{
	//	debug_appcall();
	//}
}
