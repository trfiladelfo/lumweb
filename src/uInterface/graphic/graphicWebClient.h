/*
 * graphicWebClient.h
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#ifndef GRAPHICWEBCLIENT_H_
#define GRAPHICWEBCLIENT_H_

#include "lwip/ip_addr.h"

#include "grlib/grlib.h"
#include "grlib/widget.h"

#define DELIMITOR_CHAR '$'

struct ip_addr *remoteIP;

void vLoadPage(char *uri);
void vSendData(tWidget *pWidget);

#endif /* GRAPHICWEBCLIENT_H_ */
