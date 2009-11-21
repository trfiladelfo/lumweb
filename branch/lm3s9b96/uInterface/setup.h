/*
 * setup.h
 *
 *  Created on: 14.11.2009
 *      Author: root
 */

#ifndef SETUP_H_
#define SETUP_H_

#include "lwip/ip_addr.h"

void prvSetupHardware(void);

void
DisplayIPAddress(struct ip_addr addr, unsigned long ulCol,
                 unsigned long ulRow);

#endif /* SETUP_H_ */
