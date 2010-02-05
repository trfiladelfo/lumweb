//*****************************************************************************
//
// can_fifo.c - This application uses the CAN controller to communicate with
//              device board using the CAN controllers FIFO mode.
//
// Copyright (c) 2009-2010 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 5570 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_can.h"
#include "inc/hw_types.h"
#include "driverlib/can.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "rit128x96x4.h"


//*****************************************************************************
//
// This is the main loop for the application.
//
//*****************************************************************************
int
main(void)
{
    int iIdx;
	tCANBitClkParms CANBitClk; // Canbus parameter
	tCANMsgObject sMsgObjectRx;
	unsigned char ucBufferIn[8];

    //
    // If running on Rev A2 silicon, turn the LDO voltage up to 2.75V.  This is
    // a workaround to allow the PLL to operate reliably.
    //
    if(REVISION_IS_A2)
    {
        SysCtlLDOSet(SYSCTL_LDO_2_75V);
    }

    //
    // Set the clocking to run directly from the PLL at 50MHz.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    //
    // Initialize the OLED display.
    //
    RIT128x96x4Init(1000000);

    //
    // Configure CAN 0 Pins.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeCAN(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure LED pin.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

    //
    // Turn off the LED.
    //
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);

    //
    // Enable the CAN controller.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    //
    // Reset the state of all the message object and the state of the CAN
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

    //
    // Hello!
    //
    RIT128x96x4StringDraw("CAN Test Slave", 14, 24, 15);


    /*//
    // Enable interrupts from CAN controller.
    //
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR);

    //
    // Enable interrupts for the CAN in the NVIC.
    //
    IntEnable(INT_CAN0);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();
*/

    //
    // Configure a receive object.
    //
    sMsgObjectRx.ulMsgID = (0x400);
    sMsgObjectRx.ulMsgIDMask = 0x7f8;
    sMsgObjectRx.ulFlags = MSG_OBJ_USE_ID_FILTER;
    sMsgObjectRx.ulMsgLen = 8;
    sMsgObjectRx.pucMsgData = ucBufferIn;
    CANMessageSet(CAN0_BASE, 1, &sMsgObjectRx, MSG_OBJ_TYPE_RX);



    //
    // Wait for new data to become available.
    //
    while((CANStatusGet(CAN0_BASE, CAN_STS_NEWDAT) & 1) == 0)
    {
        //
        // Read the message out of the message object.
        //
        CANMessageGet(CAN0_BASE, 1, &sMsgObjectRx, true);
        RIT128x96x4StringDraw(sMsgObjectRx.pucMsgData, 14, 44, 15);

    }
    RIT128x96x4StringDraw("GOT DATA", 14, 24, 15);


}
