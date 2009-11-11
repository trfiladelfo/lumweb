/*
 * The Main Routine to start FreeRTOS with all Tasks
 *
 * @author anzinger, hahn
 */

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

/** LWIP Includes */
#include "lwip/opt.h"
#include "lwip/udp.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

/* uInterface includes. */
#include "ethernet/ETHIsr.h"
#include "ethernet/LWIPStack.h"
#include "uart/uartstdio.h"
#include "httpd/io.h"

extern int __HEAP_START;

//#include "comTask/comTask.h"   /* include communication task header */
//#include "debugTask/debugTask.h" /* include the debugging task */
//#include "graphicsLibrary/graphicObjects.h"
//#include "graphicsLibrary/runGraphics.h" /* include the Graphics Libary */

/*-----------------------------------------------------------*/

/* The time between cycles of the 'check' functionality (defined within the
 tick hook. */
//#define mainCHECK_DELAY						( ( portTickType ) 5000 / portTICK_RATE_MS )


/*
 * Task priorities.
 */
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )

/*
 * Configure the hardware for the demo.
 */
static void prvSetupHardware(void);

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

#define mainBASIC_WEB_STACK_SIZE            ( configMINIMAL_STACK_SIZE * 3 )
/* t holds the actual time. */
time_t t;

int main(void) {

	IP_CONFIG * ipcfg;
	/* Setup the Hardware */
	prvSetupHardware();

	/* The main Communication between COMM-, GRAPH and HTTPD Task */
	//xCOMQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xCOMMessage));
	//xHTTPDQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xCOMMessage));
	//xGraphQueue = xQueueCreate(COM_QUEUE_SIZE, sizeof(xCOMMessage));

	/* Task for the Interface on the Local Display */
	//xTaskCreate(vGraphicObjectsTask, (signed portCHAR *) "graphic",
	//		mainGRAPHIC_OBJECTS_STACK_SIZE + 50, NULL, mainCHECK_TASK_PRIORITY, &xGraphicObjectsTaskHandler);

	/* Create the uIP task if running on a processor that includes a MAC and
	 PHY. */
	if (SysCtlPeripheralPresent(SYSCTL_PERIPH_ETH)) {
		ipcfg = pvPortMalloc(sizeof(IP_CONFIG));
		ipcfg->IPMode = IPADDR_USE_DHCP;
		xTaskCreate( LWIPServiceTaskInit, ( signed portCHAR * ) "lwi", mainBASIC_WEB_STACK_SIZE * 5, ipcfg, 3, NULL );
		//xTaskCreate(vHttpdTask, (signed portCHAR *) "httpd", HTTPD_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, &xHttpTaskHandler);
	}

	/* Start the Communication Task (vComTask) to interact with the machine */
	//xTaskCreate(vComTask, (signed portCHAR *) "comTask", COM_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle
	 task. */
	for (;;)
		;

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

	//
	// Enable the peripherals
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

	//
	// Enable Interrupts
	//
	IntMasterEnable();

	//
	// Set GPIO A0 and A1 as UART.
	//
	//GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void) {
	// Function that is called every Schedule Circle
}
/*-----------------------------------------------------------*/

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
		// printf("Time: %s", ctime(&t));
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

	printf("ApplicationStackOverflowHook in %s\n", pcTaskName);
	for (;;)
		;
}

/*-----------------------------------------------------------*/
void LWIPDebug(const char *pcString, ...) {
	printf(pcString);
}

/*-----------------------------------------------------------*/
void __error__(char *pcFilename, unsigned long ulLine) {
	printf("Error in %s Line %d\n", pcFilename, ulLine);
}

/*-----------------------------------------------------------*/
void NmiSRDebug(void) {
	printf("NmiSR");
	while (1)
		;
}

/*-----------------------------------------------------------*/
void IntDefaultHandlerDebug(void) {
	printf("IntDefaultHandler");
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

caddr_t _sbrk ( int incr )
{
  //static unsigned char *heap = NULL;
  //unsigned char *prev_heap;

  //if (heap == NULL) {
  //  heap = (unsigned char *)&__HEAP_START;
  //}
  //prev_heap = heap;
  /* check removed to show basic approach */

  //heap += incr;

  //return (caddr_t) prev_heap;
	usprintf()
	return 0;
}


