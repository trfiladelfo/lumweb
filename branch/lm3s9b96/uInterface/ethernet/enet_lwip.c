//*****************************************************************************
//
// enet_lwip.c - Sample WebServer Application using lwIP.
//
// Copyright (c) 2007-2009 Luminary Micro, Inc.  All rights reserved.
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
// This is part of revision 4905 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************
#include <stdlib.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_types.h"
#include "driverlib/ethernet.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "ethernet/lwiplib.h"
#include "uart/uartstdio.h"
#include "grlib/grlib.h"
#include "ethernet/httpd/httpd.h"
#include "drivers/kitronix320x240x16_ssd2119_8bit.h"
#include "drivers/set_pinout.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Ethernet with lwIP (enet_lwip)</h1>
//!
//! This example application demonstrates the operation of the Stellaris
//! Ethernet controller using the lwIP TCP/IP Stack.  DHCP is used to obtain
//! an Ethernet address.  If DHCP times out without obtaining an address,
//! AUTOIP will be used to obtain a link-local address.  The address that is
//! selected will be shown on the QVGA display.
//!
//! The file system code will first check to see if an SD card has been plugged
//! into the microSD slot.  If so, all file requests from the web server will
//! be directed to the SD card.  Otherwise, a default set of pages served up
//! by an internal file system will be used.
//!
//! For additional details on lwIP, refer to the lwIP web page at:
//! http://www.sics.se/~adam/lwip/
//
//*****************************************************************************

//*****************************************************************************
//
// Defines for setting up the system clock.
//
//*****************************************************************************
#define SYSTICKHZ               100
#define SYSTICKMS               (1000 / SYSTICKHZ)
#define SYSTICKUS               (1000000 / SYSTICKHZ)
#define SYSTICKNS               (1000000000 / SYSTICKHZ)



//*****************************************************************************
//
// Position and movement granularity for the status indicator shown while
// the IP address is being determined.
//
//*****************************************************************************
#define STATUS_X     50
#define STATUS_Y     100
#define MAX_STATUS_X (320 - (2 * STATUS_X))
#define ANIM_STEP_SIZE   8

//*****************************************************************************
//
// The application's graphics context.
//
//*****************************************************************************
tContext g_sContext;

//*****************************************************************************
//
// External Application references.
//
//*****************************************************************************
extern void fs_init(void);
extern void fs_tick(unsigned long ulTickMS);

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// Required by lwIP library to support any host-related timer functions.
//
//*****************************************************************************
void lwIPHostTimerHandler(void) {
}

//*****************************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*****************************************************************************
/*void SysTickIntHandler(void) {
	//
	// Call the lwIP timer handler.
	//
	lwIPTimer(SYSTICKMS);

	//
	// Run the file system tick handler.
	//
	fs_tick(SYSTICKMS);
}*/

//*****************************************************************************
//
// This example demonstrates the use of the Ethernet Controller.
//
//*****************************************************************************
void vLWIPServiceTaskInit(void* pvParameters) {
	unsigned long ulUser0, ulUser1;
	unsigned char pucMACArray[8];

	printf("Setup LWIP ...\n");

	//
	// Configure the hardware MAC address for Ethernet Controller filtering of
	// incoming packets.
	//
	// For the LM3S6965 Evaluation Kit, the MAC address will be stored in the
	// non-volatile USER0 and USER1 registers.  These registers can be read
	// using the FlashUserGet function, as illustrated below.
	//
	FlashUserGet(&ulUser0, &ulUser1);
	if ((ulUser0 == 0xffffffff) || (ulUser1 == 0xffffffff)) {
		printf("No MAC-Address programmed\n");
	}

	//
	// Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
	// address needed to program the hardware registers, then program the MAC
	// address into the Ethernet Controller registers.
	//
	pucMACArray[0] = ((ulUser0 >> 0) & 0xff);
	pucMACArray[1] = ((ulUser0 >> 8) & 0xff);
	pucMACArray[2] = ((ulUser0 >> 16) & 0xff);
	pucMACArray[3] = ((ulUser1 >> 0) & 0xff);
	pucMACArray[4] = ((ulUser1 >> 8) & 0xff);
	pucMACArray[5] = ((ulUser1 >> 16) & 0xff);

	printf("Set MAC to %02X:%02X:%02X:%02X:%02X:%02X\n", pucMACArray[0], pucMACArray[1], pucMACArray[2], pucMACArray[3], pucMACArray[4], pucMACArray[5]);

	//
	// Initialze the lwIP library, using DHCP.
	//
	//  lwIPInit(pucMACArray, 0, 0, 0, IPADDR_USE_DHCP);


	// IP Statisch setzen, 192.168.2.201
	lwIPInit(pucMACArray, 0xC0A802C9, 0xfffff800, 0xC0A807F5, IPADDR_USE_STATIC);
	printf("static ip: 192.168.2.201 \n");

	//
	// Initialize the sample httpd server.
	//
	printf("Initialize HTTPD\n");
	httpd_init();

	//
	// Loop forever.  All the work is done in interrupt handlers.
	//
	printf("Waiting for Packets\n");
	while (1) {
		vTaskDelay(1);
	}
}

