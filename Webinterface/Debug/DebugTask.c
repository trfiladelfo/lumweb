/*
 * DebugTask.c
 *
 *  Created on: 19.10.2009
 *      Author: root
 */

#include "hw_types.h"
#include "sysctl.h"
#include "gpio.h"
#include "hw_memmap.h"

#include "string.h"
#include "stdlib.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "DebugTask.h"
#include "SerialComm/uartstdio.h"

void vDebugTaskRun(void* pvParameters);

typedef struct {
	char *msg;
} xDebugMessage;

xQueueHandle xDebugQueue;
xDebugMessage xMessage;
xDebugMessage xReciveMessage;
xTaskHandle xDebugHandle;



void vDebugTaskRun(void* pvParameters) {
	while (1) {
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);

		while (xQueueReceive( xDebugQueue, &xReciveMessage, ( portTickType ) 1000 )
				== pdTRUE) {
			UARTprintf(xReciveMessage.msg);
			vPortFree(xReciveMessage.msg);
		}
		vTaskSuspend(xDebugHandle);
	}
}

void vDebugTaskInit(void) {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);

	IntMasterEnable();

	/* Create the queue used by the Graphics task.  Messages are received via this queue. */
	xDebugQueue = xQueueCreate(10, sizeof(xDebugMessage));
	UARTStdioInit(0);
	xTaskCreate(vDebugTaskRun, (signed portCHAR *) "DebugTask", 100, NULL, 1,
			&xDebugHandle);

}

void vDebugTaskSend(char* message) {
	int i;
	xMessage.msg = pvPortMalloc(sizeof(message));
	for (i = 0; i < sizeof(message); i++) {
		xMessage.msg[i] = message[i];
	}
	xQueueSend(xDebugQueue, &xMessage, (portTickType) 0);
	vTaskResume(xDebugHandle);
}
