/*
 * The Main Routine to start FreeRTOS with all Tasks
 *
 * @author anzinger, hahn
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>

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
#include "DebugTask/debugTask.h" /* include the debugging task */
#include "GraphicsLibrary/graphicObjects.h"
#include "GraphicsLibrary/runGraphics.h" /* include the Graphics Libary */
#include "webserver/httpd-queue.h"
#include "UipBasic/uipBasics.h"

/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
 tick hook. */
#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )

/* Size of the stack allocated to the uIP task. */
#define mainBASIC_WEB_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 5 )

/* Size of the stack allocated to the OLED task. */
#define mainGRAPHIC_OBJECTS_STACK_SIZE      ( configMINIMAL_STACK_SIZE * 3 )

/* Task priorities. */

#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainCREATOR_TASK_PRIORITY           ( tskIDLE_PRIORITY + 3 )

/* The maximum number of message that can be waiting for display at any one
 time. */
#define mainOLED_QUEUE_SIZE					( 3 )

/*
 * The task that handles the uIP stack.  All TCP/IP processing is performed in
 * this task.
 */
extern void vuIP_Task(void *pvParameters);

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

/* Variables used to detect the test in the idle hook failing. */
unsigned portLONG ulIdleError = pdFALSE;

/*-----------------------------------------------------------*/

/*************************************************************************
 * Please ensure to read http://www.freertos.org/portLM3Sxxxx_Eclipse.html
 * which provides information on configuring and running this demo for the
 * various Luminary Micro EKs.
 *************************************************************************/
int main(void)
{

	char buffer[20];

	prvSetupHardware();

	/* The main Communication between COMM-, GRAPH and HTTPD Task */
	xCOMQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xCOMMessage));
	xHTTPDQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xCOMMessage));
	xGraphQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xCOMMessage));

	xTaskCreate(vGraphicObjectsTask, (signed portCHAR *) "graphicObjects",
			mainGRAPHIC_OBJECTS_STACK_SIZE + 50, NULL, mainCHECK_TASK_PRIORITY
					- 1, &xGraphicTaskHandler);

	/* Create the uIP task if running on a processor that includes a MAC and
	 PHY. */
	if (SysCtlPeripheralPresent(SYSCTL_PERIPH_ETH))
	{
		vInitUipWithIp(192, 168, 2, 201);

		xTaskCreate(vuIP_Task, (signed portCHAR *) "uIP",
				mainBASIC_WEB_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY - 1,
				NULL);
	}

	/* Start the Communication Task (vComTask) to interact with the machine */

	xTaskCreate(vComTask, (signed portCHAR *) "comTask", COM_STACK_SIZE, NULL,
			tskIDLE_PRIORITY, NULL);

	/* Start the scheduler. */

	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	 task. */
	for (;;)
		;
	return 0;
}
/*-----------------------------------------------------------*/

void prvSetupHardware(void)
{
	/* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
	 a workaround to allow the PLL to operate reliably. */
	if (DEVICE_IS_REVA2)
	{
		SysCtlLDOSet(SYSCTL_LDO_2_75V);
	}

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
			| SYSCTL_XTAL_8MHZ);

	//
	// Enable the peripherals
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_3), GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

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

void vApplicationTickHook(void)
{
}

void vApplicationStackOverflowHook(xTaskHandle *pxTask,
		signed portCHAR *pcTaskName)
{
	for (;;)
		;
}
/*-----------------------------------------------------------*/

