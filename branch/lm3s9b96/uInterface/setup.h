/*
 * setup.h
 *
 *  Created on: 14.11.2009
 *      Author: root
 */

#ifndef SETUP_H_
#define SETUP_H_

#include "lwip/ip_addr.h"

//*****************************************************************************
//
// Interrupt priority definitions.  The top 3 bits of these values are
// significant with lower values indicating higher priority interrupts.
//
//*****************************************************************************

#define SYSTICK_INT_PRIORITY    0x80
#define ETHERNET_INT_PRIORITY   0xC0

void prvSetupHardware(void);

void
DisplayIPAddress(struct ip_addr addr, unsigned long ulCol,
                 unsigned long ulRow);

#endif /* SETUP_H_ */
