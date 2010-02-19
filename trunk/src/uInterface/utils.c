/*
 * utils.c
 *
 *  Created on: 26.01.2010
 *      Author: root
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

#include "ethernet/LWIPStack.h"

#include "configuration/configloader.h"

#include "graphic/gui/dislpayBasics.h"

#include "utils.h"

void printaddr(struct ip_addr addr) {
	printf("%d.%d.%d.%d", (unsigned int) ((addr.addr) & 0xFF), (unsigned int) ((addr.addr >> 8) & 0xFF),
			(unsigned int) ((addr.addr >> 16) & 0xFF), (unsigned int) ((addr.addr >> 24) & 0xFF));
}

void printip(struct ip_addr *addr) {
	printf("%d.%d.%d.%d", (unsigned int) ((addr->addr) & 0xFF), (unsigned int) ((addr->addr >> 8) & 0xFF),
			(unsigned int) ((addr->addr >> 16) & 0xFF), (unsigned int) ((addr->addr >> 24) & 0xFF));
}

void printnetif(struct netif *netif) {
	//struct dhcp *dhcp = netif->dhcp;
	printf("Adresskonfiguration:\n\tIP Adresse: ");
	printaddr(netif->ip_addr);
	printf("\n\tSubnet    : ");
	printaddr(netif->netmask);
	printf("\n\tGateway   : ");
	printaddr(netif->gw);
	printf("\n");
}

struct ip_addr* getAddresFromConfig(char* config) {
	char buffer[4][4];
	int i, j, pos = 0;
	struct ip_addr *retAddr = NULL;

	char* configLoad = loadFromConfig(IP_CONFIG_FILE, config);

	if (configLoad[0] < '0' && configLoad[0] > '9') {
		vShowBootText("Remote IP is not Valid!");
		retAddr = NULL;
	} else {

		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				buffer[i][j] = configLoad[pos];
				pos++;
				if (configLoad[pos] == '.') {
					j++;
					pos++;
					buffer[i][j] = 0;
					break;
				}
			}
			buffer[i][j] = 0;
		}

		retAddr = (struct ip_addr*) pvPortMalloc(sizeof(struct ip_addr));
		retAddr->addr = 0;

		for (i = 0; i < 4; i++) {
			retAddr->addr = retAddr->addr | ((atoi(buffer[i]) & 0xFF)
					<< (i * 8));
		}
	}
	vPortFree(configLoad);
	return retAddr;
}

char* pcStrdup(char *src) {
	int i, len = strlen(src);
	char* ret = (char*) pvPortMalloc(len + 1);

	if (ret != NULL) {
		for (i = 0; i < len && src[i] != 0; i++) {
			ret[i] = src[i];
		}
		ret[i] = 0;
	}
	return ret;
}

int iIsSpace (char c) {

	if (c != 0x20 && c != 0x09 && c != 0x0a && c != 0x0b && c != 0x0c && c != 0x0d) {
		return 0;
	}
	return c;
}

char* pcStrstr (char* str, char* search) {

	int i, len = strlen (str), searchlen = strlen (search);
	for (i = 0; i < len; i++) {
		if (strncmp (str + i, search, searchlen) == 0) {
			return str + i;
		}
	}
	return NULL;
}
