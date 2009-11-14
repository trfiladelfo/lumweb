/*
 FreeRTOS.org V5.1.1 - Copyright (C) 2003-2008 Richard Barry.

 This file is part of the FreeRTOS.org distribution.

 FreeRTOS.org is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 FreeRTOS.org is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with FreeRTOS.org; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 A special exception to the GPL can be applied should you wish to distribute
 a combined work that includes FreeRTOS.org, without being obliged to provide
 the source code for any proprietary components.  See the licensing section
 of http://www.FreeRTOS.org for full details of how and when the exception
 can be applied.

 ***************************************************************************
 ***************************************************************************
 *                                                                         *
 * SAVE TIME AND MONEY!  We can port FreeRTOS.org to your own hardware,    *
 * and even write all or part of your application on your behalf.          *
 * See http://www.OpenRTOS.com for details of the services we provide to   *
 * expedite your project.                                                  *
 *                                                                         *
 ***************************************************************************
 ***************************************************************************

 Please ensure to read the configuration and relevant port sections of the
 online documentation.

 http://www.FreeRTOS.org - Documentation, latest information, license and
 contact details.

 http://www.SafeRTOS.com - A version that is certified for use in safety
 critical systems.

 http://www.OpenRTOS.com - Commercial support, development, porting,
 licensing and training services.
 */

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks.
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Fast Interrupt Test" - A high frequency periodic interrupt is generated
 * using a free running timer to demonstrate the use of the
 * configKERNEL_INTERRUPT_PRIORITY configuration constant.  The interrupt
 * service routine measures the number of processor clocks that occur between
 * each interrupt - and in so doing measures the jitter in the interrupt timing.
 * The maximum measured jitter time is latched in the ulMaxJitter variable, and
 * displayed on the OLED display by the 'OLED' task as described below.  The
 * fast interrupt is configured and handled in the timertest.c source file.
 *
 * "OLED" task - the OLED task is a 'gatekeeper' task.  It is the only task that
 * is permitted to access the display directly.  Other tasks wishing to write a
 * message to the OLED send the message on a queue to the OLED task instead of
 * accessing the OLED themselves.  The OLED task just blocks on the queue waiting
 * for messages - waking and displaying the messages as they arrive.
 *
 * "Check" hook -  This only executes every five seconds from the tick hook.
 * Its main function is to check that all the standard demo tasks are still
 * operational.  Should any unexpected behaviour within a demo task be discovered
 * the tick hook will write an error to the OLED (via the OLED task).  If all the
 * demo tasks are executing with their expected behaviour then the check task
 * writes PASS to the OLED (again via the OLED task), as described above.
 *
 * "uIP" task -  This is the task that handles the uIP stack.  All TCP/IP
 * processing is performed in this task.
 */

/*************************************************************************
 * Please ensure to read http://www.freertos.org/portlm3sx965.html
 * which provides information on configuring and running this demo for the
 * various Luminary Micro EKs.
 *************************************************************************/

/* Standard includes. */
#include <stdio.h>
#include <time.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "hw_nvic.h"
#include "sysctl.h"
#include "gpio.h"
#include "grlib.h"
#include "rit128x96x4.h"

/* Demo app includes. */
#include "ethernet/ETHIsr.h"
#include "ethernet/LWIPStack.h"
#include "uart/uartstdio.h"
#include "ethernet/cgi/io.h"

#include "lwip/opt.h"
#include "lwip/udp.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

#define printf UARTprintf

/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
 tick hook. */
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )

/* Size of the stack allocated to the uIP task. */
#define mainBASIC_WEB_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 3 )

/* The OLED task uses the sprintf function so requires a little more stack too. */
#define mainOLED_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )

/* Task priorities. */
#define mainQUEUE_POLL_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainSEM_TEST_PRIORITY				( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )
#define mainINTEGER_TASK_PRIORITY           ( tskIDLE_PRIORITY )
#define mainGEN_QUEUE_TASK_PRIORITY			( tskIDLE_PRIORITY )

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

/*-----------------------------------------------------------*/

/*
 * The task that handles the lwIP stack.  All TCP/IP processing is performed in
 * this task.
 */
extern void vuIP_Task(void *pvParameters);

/*
 * The display is written two by more than one task so is controlled by a
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the display directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
static void vOLEDTask(void *pvParameters);

/*
 * The task that handles the SD Card
 */
extern void vSDcardTask(void *pvParameters);

/*
 * The task that handles the Real Time Clock
 */
static void vRealTimeClockTask(void * pvParameters);

/*
 * Configure the hardware for the demo.
 */
static void prvSetupHardware(void);

/*
 * Hook functions that can get called by the kernel.
 */
void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName);
void vApplicationTickHook(void);

/*-----------------------------------------------------------*/

/* The queue used to send messages to the OLED task. */
xQueueHandle xOLEDQueue;

/* t holds the actual time. */
time_t t;

/* The welcome text. */
const portCHAR * const pcWelcomeMessage = "   www.FreeRTOS.org";

/*-----------------------------------------------------------*/

/*************************************************************************
 * Please ensure to read http://www.freertos.org/portlm3sx965.html
 * which provides information on configuring and running this demo for the
 * various Luminary Micro EKs.
 *************************************************************************/
int main(void) {
	IP_CONFIG * ipcfg;

	prvSetupHardware();

	UARTprintf("\n\n\nStarte Programm ...\n");
	UARTprintf("Universelles Interface von Anzinger Martin und Hahn Florian\n");

	/* Create the lwIP task if running on a processor that includes a MAC and	PHY. */
	if (SysCtlPeripheralPresent(SYSCTL_PERIPH_ETH)) {
		UARTprintf("Initialisiere IP ...\n");
		ipcfg = pvPortMalloc(sizeof(IP_CONFIG));
		ipcfg->IPMode = IPADDR_USE_DHCP;
		//ipcfg->IPMode = IPADDR_USE_STATIC;
		//ipcfg->IPAddr = 0xC0A814C8; //192.168.20.200
		//ipcfg->NetMask = 0xffffff00;
		//ipcfg->GWAddr = 0xC0A81401;
		UARTprintf("Starte LWIP ...\n");
		xTaskCreate( LWIPServiceTaskInit, ( signed portCHAR * ) "lwi", (configMINIMAL_STACK_SIZE * 5 ), ipcfg, ( configMAX_PRIORITIES - 2), NULL );
	}

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	 task. */
	return 0;
}
/*-----------------------------------------------------------*/

void prvSetupHardware(void) {
	/* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
	 a workaround to allow the PLL to operate reliably. */
	if (DEVICE_IS_REVA2) {
		SysCtlLDOSet(SYSCTL_LDO_2_75V);
	}

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
			| SYSCTL_XTAL_8MHZ);
	io_init();
	UARTStdioInit(0);
	ETHServiceTaskInit(0);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {
}

/*-----------------------------------------------------------*/
void vRealTimeClockTask(void * pvParameters) {
	portTickType xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	t = 0;
	for (;;) {
		// Wait for the next cycle.
		vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
		t++;
	}
}

/*-----------------------------------------------------------*/
void vSetRealTimeClock(time_t t_new) {
	t = t_new;
}
//*****************************************************************************
//
// Return Date and Time
//
//*****************************************************************************
void get_dateandtime(char * pcBuf, int iBufLen) {
	snprintf(pcBuf, iBufLen, ctime(&t));
}
/*-----------------------------------------------------------*/
void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName) {
	(void) pxTask;
	(void) pcTaskName;

	for (;;)
		;
}

/*-----------------------------------------------------------*/
void LWIPDebug(const char *pcString, ...) {
}

/*-----------------------------------------------------------*/
void __error__(char *pcFilename, unsigned long ulLine) {
}

/*-----------------------------------------------------------*/
void NmiSRDebug(void) {
	while (1)
		;
}

/*-----------------------------------------------------------*/
void IntDefaultHandlerDebug(void) {
	while (1)
		;
}

/*-----------------------------------------------------------*/
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

extern int __HEAP_START;

caddr_t _sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL) {
		heap = (unsigned char *) &__HEAP_START;
	}
	prev_heap = heap;
	/* check removed to show basic approach */

	heap += incr;

	return (caddr_t) prev_heap;
}

