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
#include <stdlib.h>
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
// Interrupt priority definitions.  The top 3 bits of these values are
// significant with lower values indicating higher priority interrupts.
//
//*****************************************************************************
#define SYSTICK_INT_PRIORITY    0x80
#define ETHERNET_INT_PRIORITY   0xC0

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
	static unsigned long ulLastIPAddress = 0;
	static long lStarPos = 0;
	static tBoolean bIncrementing = true;
	unsigned long ulIPAddress;
	tRectangle sRect;

	ulIPAddress = lwIPLocalIPAddrGet();

	//
	// If IP Address has not yet been assigned, update the display accordingly
	//
	if (ulIPAddress == 0) {
		//
		// Update status bar on the display.  First remove the previous
		// asterisk.
		//
		GrStringDrawCentered(&g_sContext, "  ", 2, lStarPos + STATUS_X,
				STATUS_Y, true);

		//
		// Are we currently moving the asterisk right or left?
		//
		if (bIncrementing) {
			//
			// Moving right.
			//
			lStarPos += ANIM_STEP_SIZE;
			if (lStarPos >= MAX_STATUS_X) {
				//
				// We've reached the right boundary so reverse direction.
				//
				lStarPos = MAX_STATUS_X;
				bIncrementing = false;
			}
		} else {
			//
			// Moving left.
			//
			lStarPos -= ANIM_STEP_SIZE;
			if (lStarPos < 0) {
				//
				// We've reached the left boundary so reverse direction.
				//
				lStarPos = 0;
				bIncrementing = true;
			}
		}

		//
		// Draw the asterisk at the new position.
		//
		GrStringDrawCentered(&g_sContext, "*", 2, lStarPos + STATUS_X,
				STATUS_Y, true);
	}

	//
	// Check if IP address has changed, and display if it has.
	//
	else if (ulLastIPAddress != ulIPAddress) {
		ulLastIPAddress = ulIPAddress;

		//
		// Clear the status area.
		//
		sRect.sXMin = STATUS_X - 10;
		sRect.sYMin = STATUS_Y - 30;
		sRect.sXMax = MAX_STATUS_X + 10;
		sRect.sYMax = STATUS_Y + 10;
		GrContextForegroundSet(&g_sContext, ClrBlack);
		GrRectFill(&g_sContext, &sRect);

		GrContextForegroundSet(&g_sContext, ClrWhite);
		GrContextFontSet(&g_sContext, &g_sFontCmss18b);
		GrStringDraw(&g_sContext, "IP Address:", -1, 60, STATUS_Y - 20, false);
		GrStringDraw(&g_sContext, "Subnet Mask:", -1, 60, STATUS_Y, false);
		GrStringDraw(&g_sContext, "Gateway:", -1, 60, STATUS_Y + 20, false);
		DisplayIPAddress(ulIPAddress, 170, STATUS_Y - 20);
		ulIPAddress = lwIPLocalNetMaskGet();
		DisplayIPAddress(ulIPAddress, 170, STATUS_Y);
		ulIPAddress = lwIPLocalGWAddrGet();
		DisplayIPAddress(ulIPAddress, 170, STATUS_Y + 20);
	}
}

//*****************************************************************************
//
// The interrupt handler for the SysTick interrupt.
//
//*****************************************************************************
void SysTickIntHandler(void) {
	//
	// Call the lwIP timer handler.
	//
	lwIPTimer(SYSTICKMS);

	//
	// Run the file system tick handler.
	//
	fs_tick(SYSTICKMS);
}

//*****************************************************************************
//
// This example demonstrates the use of the Ethernet Controller.
//
//*****************************************************************************

void LWIPServiceTaskInit(void) {
	unsigned long ulUser0, ulUser1;
	unsigned char pucMACArray[8];

	UARTprintf("set ip \n");

	//
	// Enable and Reset the Ethernet Controller.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
	SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

	//
	// Enable the peripherals
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

	//
	// Initialize the file system.
	//
	fs_init();

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
		//
		// We should never get here.  This is an error if the MAC address has
		// not been programmed into the device.  Exit the program.
		//
		/*    GrStringDrawCentered(&g_sContext, "MAC Address", -1,
		 GrContextDpyWidthGet(&g_sContext) / 2,
		 GrContextDpyHeightGet(&g_sContext) / 2, false);
		 GrStringDrawCentered(&g_sContext, "Not Programmed!", -1,
		 GrContextDpyWidthGet(&g_sContext) / 2,
		 (GrContextDpyHeightGet(&g_sContext) / 2) + 20,
		 false);
		 while(1)
		 {
		 } */
		// TODO
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

	//
	// Initialze the lwIP library, using DHCP.
	//
	//  lwIPInit(pucMACArray, 0, 0, 0, IPADDR_USE_DHCP);


	// IP Statisch setzen, 192.168.2.201
	lwIPInit(pucMACArray, 0xC0A802C9, 0xfffff800, 0xC0A807F5, IPADDR_USE_STATIC);
	UARTprintf("static ip: 192.168.2.201 \n");

	//
	// Initialize the sample httpd server.
	//
	httpd_init();

	//
	// Set the interrupt priorities.  We set the SysTick interrupt to a higher
	// priority than the Ethernet interrupt to ensure that the file system
	// tick is processed if SysTick occurs while the Ethernet handler is being
	// processed.  This is very likely since all the TCP/IP and HTTP work is
	// done in the context of the Ethernet interrupt.
	//
	IntPriorityGroupingSet(4);
	IntPrioritySet(INT_ETH, ETHERNET_INT_PRIORITY);
	IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);

	//
	// Loop forever.  All the work is done in interrupt handlers.
	//
	while (1) {
	}
}

