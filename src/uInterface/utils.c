/*
 * utils.c
 *
 *  Created on: 26.01.2010
 *      Author: root
 */
#include "lwip/ip_addr.h"
#include "lwip/netif.h"


void printaddr(struct ip_addr addr) {
	printf("%d.%d.%d.%d", ((addr.addr) & 0xFF), ((addr.addr >> 8) & 0xFF),
			((addr.addr >> 16) & 0xFF), ((addr.addr >> 24) & 0xFF));
}

void printip(struct ip_addr *addr) {
	printf("%d.%d.%d.%d", ((addr->addr) & 0xFF), ((addr->addr >> 8) & 0xFF),
			((addr->addr >> 16) & 0xFF), ((addr->addr >> 24) & 0xFF));
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
