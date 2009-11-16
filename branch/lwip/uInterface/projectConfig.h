/*
 * projectConfig.h
 *
 * File for the Configuration of our Project
 *
 *  Created on: 14.11.2009
 *      Author: root
 */

#ifndef PROJECTCONFIG_H_
#define PROJECTCONFIG_H_

#include <stdio.h>
#include <time.h>

time_t t; // Holds the actual Time

#define USE_DHCP 			1

#define ipaddr1(addr) (htons(((u16_t *)(addr))[0]) >> 8)
#define ipaddr2(addr) (htons(((u16_t *)(addr))[0]) & 0xff)
#define ipaddr3(addr) (htons(((u16_t *)(addr))[1]) >> 8)
#define ipaddr4(addr) (htons(((u16_t *)(addr))[1]) & 0xff)

#endif /* PROJECTCONFIG_H_ */
