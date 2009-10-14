/*
 FreeRTOS.org V5.3.1 - Copyright (C) 2003-2009 Richard Barry.

 This file is part of the FreeRTOS.org distribution.

 FreeRTOS.org is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License (version 2) as published
 by the Free Software Foundation and modified by the FreeRTOS exception.
 **NOTE** The exception to the GPL is included to allow you to distribute a
 combined work that includes FreeRTOS.org without being obliged to provide
 the source code for any proprietary components.  Alternative commercial
 license and support terms are also available upon request.  See the
 licensing section of http://www.FreeRTOS.org for full details.

 FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along
 with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
 Temple Place, Suite 330, Boston, MA  02111-1307  USA.


 ***************************************************************************
 *                                                                         *
 * Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
 *                                                                         *
 * This is a concise, step by step, 'hands on' guide that describes both   *
 * general multitasking concepts and FreeRTOS specifics. It presents and   *
 * explains numerous examples that are written using the FreeRTOS API.     *
 * Full source code for all the examples is provided in an accompanying    *
 * .zip file.                                                              *
 *                                                                         *
 ***************************************************************************

 1 tab == 4 spaces!

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
 * Please ensure to read http://www.freertos.org/portLM3Sxxxx_Eclipse.html
 * which provides information on configuring and running this demo for the
 * various Luminary Micro EKs.
 *************************************************************************/

/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "sysctl.h"
#include "gpio.h"
#include "grlib.h"

#include "ComTask/comTask.h"   /* include communication task header */

/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
 tick hook. */
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )

/* Size of the stack allocated to the uIP task. */
#define mainBASIC_WEB_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 3 )

/* Size of the stack allocated to the OLED task. */
#define mainGRAPHIC_OBJECTS_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 3 )

/* The OLED task uses the sprintf function so requires a little more stack too. */
#define mainOLED_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 50 )

/* Task priorities. */

#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )

/* The maximum number of message that can be waiting for display at any one
 time. */
#define mainOLED_QUEUE_SIZE					( 3 )

/*-----------------------------------------------------------*/

/*
 * The task that handles the uIP stack.  All TCP/IP processing is performed in
 * this task.
 */
extern void vuIP_Task(void *pvParameters);

/*
 * The task that handles the graphicObjects on the LCD/OLED Screen
 */
extern void vGraphicObjectsTask(void *pvParameters);

/*
 * The display is written two by more than one task so is controlled by a
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the display directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
//static void vOLEDTask(void *pvParameters);

/*
 * Configure the hardware for the demo.
 */
static void prvSetupHardware(void);

/*
 * Configures the high frequency timers - those used to measure the timing
 * jitter while the real time kernel is executing.
 */
extern void vSetupHighFrequencyTimer(void);

/*
 * The idle hook is used to run a test of the scheduler context switch
 * mechanism.
 */
void vApplicationIdleHook(void) __attribute__((naked));
/*-----------------------------------------------------------*/

/* The queue used to send messages to the OLED task. */
xQueueHandle xOLEDQueue;

/* The welcome text. */
const portCHAR * const pcWelcomeMessage = " DebuggingScreen RTOS";

/* Variables used to detect the test in the idle hook failing. */
unsigned portLONG ulIdleError = pdFALSE;

/*-----------------------------------------------------------*/

/*************************************************************************
 * Please ensure to read http://www.freertos.org/portLM3Sxxxx_Eclipse.html
 * which provides information on configuring and running this demo for the
 * various Luminary Micro EKs.
 *************************************************************************/
int main(void) {
	prvSetupHardware();
	RIT128x96x4Init(1000000);

	SerialPortInit(0, 115200, 256, 1024);

	SerialPrintf("\n\n\r");
	SerialPrintf(
			"||===============================================================||\n\r");
	SerialPrintf(
			"|| XX      XX    XXXXXX    XX  XX      XX  XXXXXXXXXX  XX        ||\n\r");
	SerialPrintf(
			"|| XX      XX  XX      XX  XX  XXXX    XX        XX    XX        ||\n\r");
	SerialPrintf(
			"|| XX      XX  XX      XX  XX  XX XX   XX       XX     XX        ||\n\r");
	SerialPrintf(
			"|| XXXXXXXXXX  XX      XX  XX  XX  XX  XX      XX      XX        ||\n\r");
	SerialPrintf(
			"|| XX      XX  XXXXXXXXXX  XX  XX   XX XX     XX       XX        ||\n\r");
	SerialPrintf(
			"|| XX      XX  XX      XX  XX  XX    XXXX    XX        XX        ||\n\r");
	SerialPrintf(
			"|| XX      XX  XX      XX  XX  XX     XXX  XXXXXXXXXX  XXXXXXXXX ||\n\r");
	SerialPrintf(
			"||       Universelles Web- und Touchscreeninterface  v 1.0       ||\n\r");
	SerialPrintf(
			"||       Diplomarbeit von Anzinger Martin und Hahn Florian       ||\n\r");
	SerialPrintf(
			"||===============================================================||\n\r");
	SerialPrintf(
			"                D E B U G G I N G I N T E R F A C E                \n\n\r");

	SerialPrintf("Initialisiere Hardware ...\n\r");

	SerialPrintf("Initialisiere Graphics ...\n\r");

	xTaskCreate(vGraphicObjectsTask, (signed portCHAR *) "graphicObjects",
			mainGRAPHIC_OBJECTS_STACK_SIZE + 50, NULL, mainCHECK_TASK_PRIORITY
					- 1, NULL);

	/* Create the uIP task if running on a processor that includes a MAC and
	 PHY. */
	SerialPrintf("Initialisiere Webserver ...\n\r");
	if (SysCtlPeripheralPresent(SYSCTL_PERIPH_ETH)) {
		xTaskCreate(vuIP_Task, (signed portCHAR *) "uIP",
				mainBASIC_WEB_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY - 1,
				NULL);
	}

	SerialPrintf("Initialisiere ComTask ...\n\r");
	/* Start the Communication Task (vComTask) to interact with the machine */
	xTaskCreate(vComTask, (signed portCHAR *) "comTask", Com_TASK_STACK_SIZE,
			NULL, tskIDLE_PRIORITY, NULL);

	SerialPrintf("Starte TaskScheduler ...\n\r");
	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	 task. */
	for (;;)
		;
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

	/* 	Enable Port F for Ethernet LEDs
	 LED0        Bit 3   Output
	 LED1        Bit 2   Output */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_3), GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

	vParTestInitialise();

	//
	// Enable the peripherals used by this example.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//
	// Enable Interrupts
	//
	IntMasterEnable();

	//
	// Set GPIO A0 and A1 as UART.
	//
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {
	;
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName) {
	SerialPrintf("CRITICAL: Stackoverflow!\n\r");
	for (;;)
		;
}
/*-----------------------------------------------------------*/

