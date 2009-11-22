/**
 * Main File to Start the Application
 */

/* Standard includes. */
#include <stdio.h>
#include <time.h>

/* System Includes */
#include "hw_memmap.h"
#include "hw_types.h"
#include "sysctl.h"

/* RTOS includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Configuration */
#include "FreeRTOSConfig.h"
#include "projectConfig.h"
#include "queueConfig.h"
#include "taskConfig.h"
#include "setup.h"

/* Project Includes */
#include "realtime.h"
#include "communication/comTask.h"
#include "graphic/graphicTask.h"

/* Globals */
extern int _pvHeapStart; // used for _sbrk defined in standalone.ld

extern LWIPServiceTaskInit();
/*-------- MAIN ---------------------------------------------*/
int main(void) {
	/* Variables */
	//	IP_CONFIG * ipcfg;

	// Setup the Hardware
	prvSetupHardware();

	printf(
			"\n\n\nUniversal Interface by Anzinger Martin and Hahn Florian (c) 2009/10\n\n");

	printf("Starting Firmware ...\n");

	// Queue Definition
	/* The main Communication between COMM-, GRAPH and HTTPD Task */
	printf("Initialize Queues ...\n\txComQueue\n");
	xComQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xComMessage));
	printf("\txHttpdQueue\n");
	xHttpdQueue = xQueueCreate(HTTPD_QUEUE_SIZE, sizeof(xComMessage));
	printf("\txGraphQueue\n");
	xGraphQueue = xQueueCreate(GRAPH_QUEUE_SIZE, sizeof(xComMessage));

	// Tasks
	printf("Starting RealTimeClock ... ");
	xTaskCreate( vRealTimeClockTask, TIME_TASK_NAME, TIME_STACK_SIZE, NULL, TIME_TASK_PRIORITY, &xRealtimeTaskHandle );
	printf("ok\n");
	printf("Starting Communication Task ... ");
	xTaskCreate( vComTask, COM_TASK_NAME, COM_STACK_SIZE, NULL, COM_TASK_PRIORITY, &xComTaskHandle);
	printf("ok\n");
	printf("Starting Graphic Task ... ");
	xTaskCreate( vGraphicTask, GRAPH_TASK_NAME, GRAPH_STACK_SIZE, NULL, GRAPH_TASK_PRIORITY, &xGraphTaskHandle );
	printf("ok\n");

	/*Create the lwIP task if running on a processor that includes a MAC and	PHY. */
	if (SysCtlPeripheralPresent(SYSCTL_PERIPH_ETH)) {
		printf("Network Interface Available\nStarting LWIP and HTTPD... ");
		xTaskCreate(vLWIPServiceTaskInit, LWIP_TASK_NAME, LWIP_STACK_SIZE, NULL, LWIP_TASK_PRIORITY, &xLwipTaskHandle );
		printf("ok\n");
	}

	/* Start the scheduler. */
	printf("Starte Scheduler ...\n\n");
	vTaskStartScheduler();

	// Loop only used if Scheduler Fails
	while (1)
		;
	return 0;
}

/*-------------EXTERN ROTS ROUTINES--------------------------*/
void vApplicationTickHook(void) {
	// Function that is called every Schedule Circle

	// Call the lwIP timer handler.
	//
	lwIPTimer(xTaskGetTickCount());

	//
	// Run the file system tick handler.
	//
	fs_tick(xTaskGetTickCount());
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName) {
	// Function that is called if there is any StackOverflow
	printf("Stack Overflow %s", pcTaskName);
	for (;;)
		;
}

// hard fault handler in C,
// with stack frame location as input parameter
void hard_fault_handler_c(unsigned int * hardfault_args) {
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;

	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);

	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);

	printf("[Hard fault handler]\n");
	printf("R0 = %x\n", stacked_r0);
	printf("R1 = %x\n", stacked_r1);
	printf("R2 = %x\n", stacked_r2);
	printf("R3 = %x\n", stacked_r3);
	printf("R12 = %x\n", stacked_r12);
	printf("LR = %x\n", stacked_lr);
	printf("PC = %x\n", stacked_pc);
	printf("PSR = %x\n", stacked_psr);
	printf("BFAR = %x\n", (*((volatile unsigned long *) (0xE000ED38))));
	printf("CFSR = %x\n", (*((volatile unsigned long *) (0xE000ED28))));
	printf("HFSR = %x\n", (*((volatile unsigned long *) (0xE000ED2C))));
	printf("DFSR = %x\n", (*((volatile unsigned long *) (0xE000ED30))));
	printf("AFSR = %x\n", (*((volatile unsigned long *) (0xE000ED3C))));

	while (1)
		; // terminate
}

/*
 * Routine with a pointer to the actual Position of the Heap
 */
caddr_t _sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		heap = (unsigned char *) &_pvHeapStart;
	}
	prev_heap = heap;
	/* check removed to show basic approach */

	heap += incr;

	return (caddr_t) prev_heap;
}
/*-----------------------------------------------------------*/
