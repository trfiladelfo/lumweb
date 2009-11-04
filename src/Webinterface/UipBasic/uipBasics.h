/*
 * uipBasics.h
 *
 *  Created on: 04.11.2009
 *      Author: root
 */

#ifndef UIPBASICS_H_
#define UIPBASICS_H_

#include "timer.h"
#ifndef UIP_APPCALL
#define UIP_APPCALL  vUipAppcall
#endif

struct timer periodic_timer, arp_timer;

unsigned char ucIp1;
unsigned char ucIp2;
unsigned char ucIp3;
unsigned char ucIp4;

void vInitUip(void);

void vUipAppcall(void);

void vUipSetIp(char ip1, char ip2, char ip3, char ip4);

void vUipSetIp(char ip1, char ip2, char ip3, char ip4);

#endif /* UIPBASICS_H_ */
