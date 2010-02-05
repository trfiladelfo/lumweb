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
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "interrupt.h"
#include "hw_ints.h"

#include "hw_can.h"
#include "can.h"


void prvSetupHardware(void) {

	tCANBitClkParms CANBitClk; // Canbus parameter


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
	IntMasterEnable();

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
	// Initialize the file system.
	//
	fs_init();

	/*---------------- CAN BUS -------------------------------------- */

	// Configure CAN Pins for PORT A
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	// Enable CAN Controller
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

	//
	// Reset the state of all the message objects and the state of the CAN
	// module to a known state.
	//
	CANInit(CAN0_BASE);

	//
	// Configure the controller for 1 Mbit operation.
	//
	CANBitClk.uSyncPropPhase1Seg = 5;
	CANBitClk.uPhase2Seg = 2;
	CANBitClk.uQuantumPrescaler = 1;
	CANBitClk.uSJW = 2;
	CANSetBitTiming(CAN0_BASE, &CANBitClk);
	//
	// Take the CAN0 device out of INIT state.
	//
	CANEnable(CAN0_BASE);


}
