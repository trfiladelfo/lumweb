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
#include "kitronix320x240x16_ssd2119_8bit.h"
#include "lwip/ip_addr.h"

tContext g_sContext;

void prvSetupHardware(void)
{
	tRectangle sRect;
	/* If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
	 a workaround to allow the PLL to operate reliably. */
	if (DEVICE_IS_REVA2) {
		SysCtlLDOSet(SYSCTL_LDO_2_75V);
	}

	/* Set the clocking to run from the PLL at 50 MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
				| SYSCTL_XTAL_16MHZ);

	PinoutSet();


    //
    // Initialize the display driver.
    //
    Kitronix320x240x16_SSD2119Init();

    //
    // Initialize the graphics context.
    //
    GrContextInit(&g_sContext, &g_sKitronix320x240x16_SSD2119);

    //
    // Fill the top 24 rows of the screen with blue to create the banner.
    //
    sRect.sXMin = 0;
    sRect.sYMin = 0;
    sRect.sXMax = GrContextDpyWidthGet(&g_sContext) - 1;
    sRect.sYMax = 23;
    GrContextForegroundSet(&g_sContext, ClrDarkBlue);
    GrRectFill(&g_sContext, &sRect);

    //
    // Put a white box around the banner.
    //
    GrContextForegroundSet(&g_sContext, ClrWhite);
    GrRectDraw(&g_sContext, &sRect);

    //
    // Put the application name in the middle of the banner.
    //
    GrContextFontSet(&g_sContext, &g_sFontCm20);
    GrStringDrawCentered(&g_sContext, "enet-lwip", -1,
                         GrContextDpyWidthGet(&g_sContext) / 2, 10, 0);

    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioInit(0);

	ETHServiceTaskInit(0);

	IntMasterEnable();
}

//*****************************************************************************
//
// Display an lwIP type IP Address.
//
//*****************************************************************************
void
DisplayIPAddress(struct ip_addr addr, unsigned long ulCol,
                 unsigned long ulRow)
{
    char pucBuf[16];

    //
    // Convert the IP Address into a string.
    //
    sprintf(pucBuf, "%d.%d.%d.%d", ((addr.addr) & 0xFF), ((addr.addr >> 8) & 0xFF),
        			((addr.addr >> 16) & 0xFF), ((addr.addr >> 24) & 0xFF));
    //
    // Display the string.
    //
    GrContextFontSet(&g_sContext, &g_sFontCmss18b);
    GrStringDraw(&g_sContext, pucBuf, -1, ulCol, ulRow, true);
}

