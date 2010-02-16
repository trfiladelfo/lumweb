/*
 * utils.h
 *
 *  Created on: 26.01.2010
 *      Author: root
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "lwip/ip_addr.h"

void printnetif (struct netif *aktNetif);
void printaddr (struct ip_addr addr);
void printip (struct ip_addr *addr);
struct ip_addr* getAddresFromConfig (char* config);
char* pcStrdup (char *src);
//int isspace (char c);

#endif /* UTILS_H_ */
