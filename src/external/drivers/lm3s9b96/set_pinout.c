//*****************************************************************************
//
// set_pinout.c - Functions related to configuration of the device pinout.
//
// Copyright (c) 2009 Luminary Micro, Inc.  All rights reserved.
// Software License Agreement
// 
// Luminary Micro, Inc. (LMI) is supplying this software for use solely and
// exclusively on LMI's microcontroller products.
// 
// The software is owned by LMI and/or its suppliers, and is protected under
// applicable copyright laws.  All rights are reserved.  You may not combine
// this software with "viral" open-source software in order to form a larger
// program.  Any use in violation of the foregoing restrictions may subject
// the user to criminal sanctions under applicable laws, as well as to civil
// liability for the breach of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
// CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 4905 of the DK-LM3S9B96 Firmware Package.
//
//*****************************************************************************

#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "sysctl.h"
#include "i2c.h"
#include "gpio.h"
#include "epi.h"
#include "set_pinout.h"

//*****************************************************************************
//
//! \addtogroup set_pinout_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Use the following to specify the GPIO pins used by the EPI bus (EPI0S00
// through EPI0S19 and EPI0S28 through EPI0S31).  The basic definitions
// set up the pins required for the SDRAM daughter board.
//
//*****************************************************************************
#define EPI_PORTA_PINS          0x00
#define EPI_PORTB_PINS_SDRAM    0x00
#define EPI_PORTC_PINS          0xf0
#define EPI_PORTD_PINS_SDRAM    0x00
#define EPI_PORTE_PINS          0x03
#define EPI_PORTF_PINS          0x30
#define EPI_PORTG_PINS          0x83
#define EPI_PORTH_PINS_SDRAM    0x3f
#define EPI_PORTJ_PINS          0x7f

//*****************************************************************************
//
// Use the following to specify the GPIO pin used by the SDRAM clock (EPI0S31).
//
//*****************************************************************************
#define EPI_CLK_PORT            GPIO_PORTG_BASE
#define EPI_CLK_PIN             0x80

//*****************************************************************************
//
// A global variable indicating which daughter board, if any, is currently
// connected to the lm3s9b96 development board.
//
//*****************************************************************************
tDaughterBoard g_eDaughterType;

//*****************************************************************************
//
// Determines which daughter board is currently attached to the lm3s9b96
// development board.
//
//*****************************************************************************
static tDaughterBoard
DaughterBoardTypeGet(void)
{
    return(DAUGHTER_NONE);
}

//*****************************************************************************
//
// Configures all pins associated with the Extended Peripheral Interface (EPI).
//
// \param eDaughter identifies the attached daughter board (if any).
//
// This function configures all pins forming part of the EPI on the device and
// configures the EPI peripheral appropriately for whichever hardware we
// detect is connected to it. On exit, the EPI peripheral is enabled and all
// pins associated with the interface are configured as EPI signals. Drive
// strength is set to 2mA for all pins other than the clock whose strength is
// configured to 8mA.
//
//*****************************************************************************
static void
EPIPinConfigSet(tDaughterBoard eDaughter)
{
    //
    // Enable the EPI peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EPI0);

    //
    // Configure the EPI pins that are common to SDRAM and HostBus8 modes.
    //
#if (EPI_PORTA_PINS != 0x00)
    GPIOPadConfigSet(GPIO_PORTA_BASE, EPI_PORTA_PINS, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTA_BASE, EPI_PORTA_PINS, GPIO_DIR_MODE_HW);
#endif
#if (EPI_PORTC_PINS != 0x00)
    GPIOPadConfigSet(GPIO_PORTC_BASE, EPI_PORTC_PINS, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTC_BASE, EPI_PORTC_PINS, GPIO_DIR_MODE_HW);
#endif
#if EPI_PORTE_PINS
    GPIOPadConfigSet(GPIO_PORTE_BASE, EPI_PORTE_PINS, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTE_BASE, EPI_PORTE_PINS, GPIO_DIR_MODE_HW);
#endif
#if EPI_PORTF_PINS
    GPIOPadConfigSet(GPIO_PORTF_BASE, EPI_PORTF_PINS, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTF_BASE, EPI_PORTF_PINS, GPIO_DIR_MODE_HW);
#endif
#if EPI_PORTG_PINS
    GPIOPadConfigSet(GPIO_PORTG_BASE, EPI_PORTG_PINS, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTG_BASE, EPI_PORTG_PINS, GPIO_DIR_MODE_HW);
#endif
#if EPI_PORTJ_PINS
    GPIOPadConfigSet(GPIO_PORTJ_BASE, EPI_PORTJ_PINS, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPU);
    GPIODirModeSet(GPIO_PORTJ_BASE, EPI_PORTJ_PINS, GPIO_DIR_MODE_HW);
#endif

    //
    // Configure the pins which are different depending upon the daughter board
    // which is attached.
    //
    switch(g_eDaughterType)
    {
        //
        // Case for the SDRAM daughter board.  The SDRAM daughter board is
        // considered the default.  It has no way to determine whether or not
        // it is present other than to configure it and attempt to read and
        // write the memory.
        //
        case DAUGHTER_NONE:
        default:
        {
#if (EPI_PORTB_PINS_SDRAM != 0x00)
            GPIOPadConfigSet(GPIO_PORTB_BASE, EPI_PORTB_PINS_SDRAM,
                             GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
            GPIODirModeSet(GPIO_PORTB_BASE, EPI_PORTB_PINS_SDRAM,
                           GPIO_DIR_MODE_HW);
#endif
#if (EPI_PORTD_PINS_SDRAM != 0x00)
            GPIOPadConfigSet(GPIO_PORTD_BASE, EPI_PORTD_PINS_SDRAM,
                             GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
            GPIODirModeSet(GPIO_PORTD_BASE, EPI_PORTD_PINS_SDRAM,
                           GPIO_DIR_MODE_HW);
#endif
#if EPI_PORTH_PINS_SDRAM
            GPIOPadConfigSet(GPIO_PORTH_BASE, EPI_PORTH_PINS_SDRAM,
                             GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
            GPIODirModeSet(GPIO_PORTH_BASE, EPI_PORTH_PINS_SDRAM,
                           GPIO_DIR_MODE_HW);
#endif

            //
            // Set the EPI operating mode for the SDRAM daughter board
            // supplied with the dk-lm3s9b96 board.  The values used here assume
            // that the core is running at between 50 and 100MHz.  This may be
            // overridden when SDRAMInit() is called later by the application.
            //
            EPIDividerSet(EPI0_BASE, 0);
            EPIModeSet(EPI0_BASE, EPI_MODE_SDRAM);
            EPIConfigSDRAMSet(EPI0_BASE, (EPI_SDRAM_CORE_FREQ_50_100 |
                              EPI_SDRAM_FULL_POWER | EPI_SDRAM_SIZE_64MBIT),
                              1024);
            EPIAddressMapSet(EPI0_BASE, (EPI_ADDR_RAM_SIZE_512MB |
                             EPI_ADDR_RAM_BASE_6));
        }
        break;
    }

#if defined(EPI_CLK_PORT) && defined(EPI_CLK_PIN)
    GPIOPadConfigSet(EPI_CLK_PORT, EPI_CLK_PIN, GPIO_STRENGTH_8MA,
                     GPIO_PIN_TYPE_STD_WPU);
#endif
}

//*****************************************************************************
//
//! Configures the LM3S9B96 device pinout for the development board.
//!
//! This function configures each pin of the lm3s9b96 device to route the
//! appropriate peripheral signal as required by the design of the
//! dk-lm3s9b96 development board.
//!
//! \return None.
//
//*****************************************************************************
void
PinoutSet(void)
{
    //
    // Enable all GPIO banks.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

    //
    // Determine which daughter board (if any) is currently attached to the
    // development board.
    //
    g_eDaughterType = DaughterBoardTypeGet();

    //
    // GPIO Port A pins
    //
    // To use CAN0, this register value must be changed. The value here
    // enables USB functionality instead of CAN. For CAN, use....
    //
    //  HWREG(GPIO_PORTA_BASE + GPIO_O_PCTL) = GPIO_PCTL_PA0_U0RX |
    //                                         GPIO_PCTL_PA1_U0TX |
    //                                         GPIO_PCTL_PA2_SSI0CLK |
    //                                         GPIO_PCTL_PA3_SSI0FSS |
    //                                         GPIO_PCTL_PA4_SSI0RX |
    //                                         GPIO_PCTL_PA5_SSI0TX |
    //                                         GPIO_PCTL_PA6_CAN0RX |
    //                                         GPIO_PCTL_PA7_CAN0TX;
    //
    //
    HWREG(GPIO_PORTA_BASE + GPIO_O_PCTL) = GPIO_PCTL_PA0_U0RX |
                                           GPIO_PCTL_PA1_U0TX |
                                           GPIO_PCTL_PA2_SSI0CLK |
                                           GPIO_PCTL_PA3_SSI0FSS |
                                           GPIO_PCTL_PA4_SSI0RX |
                                           GPIO_PCTL_PA5_SSI0TX |
                                           GPIO_PCTL_PA6_USB0EPEN |
                                           GPIO_PCTL_PA7_USB0PFLT;

    //
    // GPIO Port B pins
    //
    HWREG(GPIO_PORTB_BASE + GPIO_O_PCTL) = GPIO_PCTL_PB2_I2C0SCL |
                                           GPIO_PCTL_PB3_I2C0SDA |
                                           GPIO_PCTL_PB4_EPI0S23 |
                                           GPIO_PCTL_PB5_EPI0S22 |
                                           GPIO_PCTL_PB6_I2S0TXSCK |
                                           GPIO_PCTL_PB7_NMI;

    //
    // GPIO Port C pins
    //
    HWREG(GPIO_PORTC_BASE + GPIO_O_PCTL) = GPIO_PCTL_PC0_TCK |
                                           GPIO_PCTL_PC1_TMS |
                                           GPIO_PCTL_PC2_TDI |
                                           GPIO_PCTL_PC3_TDO |
                                           GPIO_PCTL_PC4_EPI0S2 |
                                           GPIO_PCTL_PC5_EPI0S3 |
                                           GPIO_PCTL_PC6_EPI0S4 |
                                           GPIO_PCTL_PC7_EPI0S5;

    //
    // GPIO Port D pins.
    //
    HWREG(GPIO_PORTD_BASE + GPIO_O_PCTL) = GPIO_PCTL_PD0_I2S0RXSCK |
                                           GPIO_PCTL_PD1_I2S0RXWS |
                                           GPIO_PCTL_PD2_EPI0S20 |
                                           GPIO_PCTL_PD3_EPI0S21 |
                                           GPIO_PCTL_PD4_I2S0RXSD |
                                           GPIO_PCTL_PD5_I2S0RXMCLK;

    //
    // GPIO Port E pins
    //
    HWREG(GPIO_PORTE_BASE + GPIO_O_PCTL) = GPIO_PCTL_PE0_EPI0S8 |
                                           GPIO_PCTL_PE1_EPI0S9 |
                                           GPIO_PCTL_PE2_EPI0S24 |
                                           GPIO_PCTL_PE3_EPI0S25 |
                                           GPIO_PCTL_PE4_I2S0TXWS |
                                           GPIO_PCTL_PE5_I2S0TXSD;

    //
    // GPIO Port F pins
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_PCTL) = GPIO_PCTL_PF1_I2S0TXMCLK |
                                           GPIO_PCTL_PF2_LED1 |
                                           GPIO_PCTL_PF3_LED0 |
                                           GPIO_PCTL_PF4_EPI0S12 |
                                           GPIO_PCTL_PF5_EPI0S15;

    //
    // GPIO Port G pins
    //
    HWREG(GPIO_PORTG_BASE + GPIO_O_PCTL) = GPIO_PCTL_PG0_EPI0S13 |
                                           GPIO_PCTL_PG1_EPI0S14 |
                                           GPIO_PCTL_PG7_EPI0S31;

    //
    // GPIO Port H pins
    //
    HWREG(GPIO_PORTH_BASE + GPIO_O_PCTL) = GPIO_PCTL_PH0_EPI0S6 |
                                           GPIO_PCTL_PH1_EPI0S7 |
                                           GPIO_PCTL_PH2_EPI0S1 |
                                           GPIO_PCTL_PH3_EPI0S0 |
                                           GPIO_PCTL_PH4_EPI0S10 |
                                           GPIO_PCTL_PH5_EPI0S11 |
                                           GPIO_PCTL_PH6_EPI0S26 |
                                           GPIO_PCTL_PH7_EPI0S27;
    //
    // GPIO Port J pins
    //
    HWREG(GPIO_PORTJ_BASE + GPIO_O_PCTL) = GPIO_PCTL_PJ0_EPI0S16 |
                                           GPIO_PCTL_PJ1_EPI0S17 |
                                           GPIO_PCTL_PJ2_EPI0S18 |
                                           GPIO_PCTL_PJ3_EPI0S19 |
                                           GPIO_PCTL_PJ4_EPI0S28 |
                                           GPIO_PCTL_PJ5_EPI0S29 |
                                           GPIO_PCTL_PJ6_EPI0S30;

  //
  // Set the pin configuration for the Extended Peripheral Interface (EPI)
  //
  EPIPinConfigSet(g_eDaughterType);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
