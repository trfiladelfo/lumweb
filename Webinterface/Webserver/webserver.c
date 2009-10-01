/*
 * webserver.c
 *
 * Start routine for the Webserver (LWIP)
 *
 *  Created on: 01.10.2009
 *      Author: anzinger
 */

#include "lwip/tcpip.h"
#include "lwiplib.h"
#include "lwip/netif.h"

#include "LWIPStack.h"
#include "ETHIsr.h"

#include "rit128x96x4.h"

void vEthernetTask(void *pvParameters)
{
	IP_CONFIG ipconfig;

	ETHServiceTaskInit(0);
	ETHServiceTaskFlush(0,ETH_FLUSH_RX | ETH_FLUSH_TX);

	ipconfig.IPMode = IPADDR_USE_STATIC;
	ipconfig.IPAddr=0xC0A80102;
	ipconfig.NetMask=0xFFFFFF00;
	ipconfig.GWAddr=0xC0A80001;

	RIT128x96x4Init(1000000);
	RIT128x96x4StringDraw("test!", 0, 85, 0xF);


    //dhcp_start();
    //dhcp_renew();
    //dhcp_release();
    //dhcp_stop();
    //dhcp_inform();


	LWIPServiceTaskInit((void *)&ipconfig);

	for(;;)
	{
	}
}
