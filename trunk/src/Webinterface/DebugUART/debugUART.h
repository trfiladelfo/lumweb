/*
 * debugUART.h
 *
 *  Created on: 06.10.2009
 *      Author: root
 */
#include "hw_types.h"

#ifndef DEBUGUART_H_
#define DEBUGUART_H_

/*
 * Starts the DebugTask
 */
void vDebugTask(void* pvParameters);
/*
 * Send a Message to the UART
 */
void vSendDebugUART(unsigned char *pucBuffer);

#endif /* DEBUGUART_H_ */
