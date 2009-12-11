/**
 * File with all configuration Routines
 */

#include "setup.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "sysctl.h"
#include "gpio.h"
#include "grlib.h"
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "interrupt.h"
#include "hw_ints.h"
#include "widget.h"

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

	UARTStdioInit(0);
	ETHServiceTaskInit(0);

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
	//Kitronix320x240x16_SSD2119Init();

	//
	// Initialize the touch screen driver and have it route its messages to the
	// widget tree.
	//
	//TouchScreenInit();
	//TouchScreenCallbackSet(WidgetPointerMessage);

	//
	// Initialize the file system.
	//
	fs_init();

	//IntMasterEnable();
}
