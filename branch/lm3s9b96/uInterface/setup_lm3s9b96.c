/**
 * File with all configuration Routines
 */

#include "setup.h"

/* Hardware library includes. */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
#include "sysctl.h"
#include "gpio.h"
#include "grlib/grlib.h"
#include "touch.h"
#include "grlib/widget.h"
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "driverlib/ethernet.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "lwip/ip_addr.h"

void prvSetupHardware(void) {
	/* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
	 a workaround to allow the PLL to operate reliably. */
	if (DEVICE_IS_REVA2) {
		SysCtlLDOSet(SYSCTL_LDO_2_75V);
	}

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
			| SYSCTL_XTAL_16MHZ);

	PinoutSet();

	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioInit(0);


	//
	// Set the interrupt priorities.  We set the SysTick interrupt to a higher
	// priority than the Ethernet interrupt to ensure that the file system
	// tick is processed if SysTick occurs while the Ethernet handler is being
	// processed.  This is very likely since all the TCP/IP and HTTP work is
	// done in the context of the Ethernet interrupt.
	//
	IntPriorityGroupingSet(4);
	IntPrioritySet(INT_ETH, ETHERNET_INT_PRIORITY);
	IntPrioritySet(FAULT_SYSTICK, SYSTICK_INT_PRIORITY);

	//
	// Initialize GUI
	//
	Kitronix320x240x16_SSD2119Init();

	//
	// Initialize the touch screen driver and have it route its messages to the
	// widget tree.
	//
	TouchScreenInit();
	TouchScreenCallbackSet(WidgetPointerMessage);

	//
	// Enable and Reset the Ethernet Controller.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
	SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

	//
	// Enable the peripherals
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//
	// Initialize the Leds on the
	//
	GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_DIR_MODE_HW);
	GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3),
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

	//
	// Initialize the file system.
	//
	fs_init();

	IntMasterEnable();
}
