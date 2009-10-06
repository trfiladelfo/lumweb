/*
 * debugUART.h
 *
 *  Created on: 06.10.2009
 *      Author: root
 */

#ifndef DEBUGUART_H_
#define DEBUGUART_H_

/*
 * Initializes the UART
 */
void vInitDebug(void);
/*
 * Send a Message to the UART
 */
void vSendDebugUART(unsigned char *pucBuffer);

#endif /* DEBUGUART_H_ */
