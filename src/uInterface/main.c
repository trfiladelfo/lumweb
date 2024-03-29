/**
 * \addtogroup System
 * @{
 *
 * \author Anziner, Hahn
 * \brief Contains the starting point for the application
 *
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
#include "queueConfig.h"
#include "taskConfig.h"
#include "setup.h"

/* Project Includes */
#include "utils.h"
#include "realtime.h"
#include "communication/comTask.h"
#include "ethernet/LWIPStack.h"
#include "graphic/graphicTask.h"
#include "log/logging.h"

#include "taglib/tags.h"

/**
 *
 * The entry point for the application.
 * It is responsible for creating queues, starting tasks and running the
 * main scheduler
 *
 */
int main(void)
{
	//
	// Setup the Hardware
	//
	prvSetupHardware();

	//
	// start Logging
	//
	UARTprintf("Init log file: Status = %d\n", initLog());
	appendToLog("Starting Firmware");
	appendToLog("Universelles Interface von Anzinger Martin und Hahn Florian");

	//
	// write welcome text to the debug console
	//
	printf("\n\n\nStarte Programm ...\n");
	printf("Universelles Interface von Anzinger Martin und Hahn Florian\n");
	printf("Starting Firmware ...\n");

	//
	// initialize Taglibrary
	//
	printf("Initialisiere Taglib ...");
	vInitTagLibrary();
	printf(" done\n");

	//
	// Queue Definition
	// The main Communication between COMM-, GRAPH and HTTPD Task
	//
	printf("Initialize Queues ...\n\txComQueue\n");
	xComQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xComMessage));
	printf("\txHttpdQueue\n");
	xHttpdQueue = xQueueCreate(HTTPD_QUEUE_SIZE, sizeof(xComMessage));
	appendToLog("Queues created");

	//
	// start Tasks
	//

	//
	//Real Time Clock Task
	//
	printf("Starting RealTimeClock ... ");
	xTaskCreate( vRealTimeClockTask, (const signed char * const)TIME_TASK_NAME, TIME_STACK_SIZE, NULL, TIME_TASK_PRIORITY, &xRealtimeTaskHandle );
	printf("ok\n");
	appendToLog("RealTimeClock started");

	//
	// Communication Task
	//
	printf("Starting Communication Task ... ");
	xTaskCreate( vComTask, (const signed char * const)COM_TASK_NAME, COM_STACK_SIZE, NULL, COM_TASK_PRIORITY, &xComTaskHandle);
	printf("ok\n");
	appendToLog("ComTask started");

	//
	// Check if an Ethernet Port is available
	//
	if (SysCtlPeripheralPresent(SYSCTL_PERIPH_ETH))
	{
		//
		// LWIP Task
		//
		printf("Starting LWIP ...\n");
		xTaskCreate( LWIPServiceTaskInit, (const signed char * const)LWIP_TASK_NAME, LWIP_STACK_SIZE, NULL, LWIP_TASK_PRIORITY, &xLwipTaskHandle );
		appendToLog("IP-Stack started");
	}

#if ENABLE_GRAPHIC
	//
	// Graphic Task
	//
	printf("Starting Graphic Task ... ");
	xTaskCreate( vGraphicTask, (const signed char * const)GRAPH_TASK_NAME, GRAPH_STACK_SIZE, NULL, GRAPH_TASK_PRIORITY, &xGraphTaskHandle );
	printf("ok\n");
#endif

	//
	// Starting the scheduler.
	//
	vTaskStartScheduler();

	//
	// Fallbackloop if Scheduler Fails
	//
	while (1)
		;
	return 0;
}

/*-------------EXTERN ROTS ROUTINES--------------------------*/
/**
 * Callbackfunction for FreeRTOS.
 * It's called every Schedule Circle
 */
void vApplicationTickHook(void)
{
	//
	// Function that is called every Schedule Circle
	//
}

/**
 * Callbackfunction for FreeRTOS
 * It's called if a Task has a Stack Overflow
 *
 * @param pxTask taskhandler for the task with the overflow
 * @param pcTaskName name of the task
 */
void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName)
{
	//
	// Function that is called if there is any StackOverflow
	//
	printf("Task %s Stackoverflow\n", pcTaskName);
	for (;;)
		;
}

/**
 * hard fault handler in C,
 * with stack frame location as input parameter
 *
 * @param hardfault_args stack frame location
 */
void hard_fault_handler_c(unsigned int * hardfault_args)
{
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
	printf("BFAR = %x\n", (*((unsigned int *) (0xE000ED38))));
	printf("CFSR = %x\n", (*((unsigned int *) (0xE000ED28))));
	printf("HFSR = %x\n", (*((unsigned int *) (0xE000ED2C))));
	printf("DFSR = %x\n", (*((unsigned int *) (0xE000ED30))));
	printf("AFSR = %x\n", (*((unsigned int *) (0xE000ED3C))));

	while (1)
		;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
