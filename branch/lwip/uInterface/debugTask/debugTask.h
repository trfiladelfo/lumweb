/*
 * debugUART.h
 *
 *  Created on: 06.10.2009
 *      Author: root
 */



#ifndef DEBUGUART_H_
#define DEBUGUART_H_

#include "FreeRTOS.h"
//#include "task.h"
#include "queue.h"
#include "hw_types.h"

//#include "psock.h"

//#define DEBUG_PORT 8962

typedef struct
{
	char *msg;
} xDebugTaskMessage;

xQueueHandle xDebugQueue;

//xTaskHandle xDebugTask;

/*struct debug_state {
  struct psock p;
  char inputbuffer[10];
  char name[40];
};*/

void vInitDebug(void);

/*
 * Starts the DebugTask
 */
//void vDebugTask(void* pvParameters);
/*
 * Send a Message to the UART
 */
void vSendDebug(unsigned char *pucBuffer);

//void debug_appcall(void);

#endif /* DEBUGUART_H_ */
