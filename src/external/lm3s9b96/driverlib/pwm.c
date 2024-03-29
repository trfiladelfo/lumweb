//*****************************************************************************
//
// pwm.c - API for the PWM modules
//
// Copyright (c) 2005-2009 Luminary Micro, Inc.  All rights reserved.
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
// This is part of revision 4905 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup pwm_api
//! @{
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_pwm.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/pwm.h"

//*****************************************************************************
//
// Misc macros for manipulating the encoded generator and output defines used
// by the API.
//
//*****************************************************************************
#define PWM_GEN_BADDR(_mod_, _gen_)                                           \
                                ((_mod_) + (_gen_))
#define PWM_GEN_EXT_BADDR(_mod_, _gen_)                                       \
                                ((_mod_) + PWM_GEN_EXT_0 +                    \
                                 ((_gen_) - PWM_GEN_0) * 2)
#define PWM_OUT_BADDR(_mod_, _out_)                                           \
                                ((_mod_) + ((_out_) & 0xFFFFFFC0))
#define PWM_IS_OUTPUT_ODD(_out_)                                              \
                                ((_out_) & 0x00000001)

//*****************************************************************************
//
//! \internal
//! Checks a PWM generator number.
//!
//! \param ulGen is the generator number.
//!
//! This function determines if a PWM generator number is valid.
//!
//! \return Returnes \b true if the generator number is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
PWMGenValid(unsigned long ulGen)
{
    return((ulGen == PWM_GEN_0) || (ulGen == PWM_GEN_1) ||
           (ulGen == PWM_GEN_2) || (ulGen == PWM_GEN_3));
}
#endif

//*****************************************************************************
//
//! \internal
//! Checks a PWM output number.
//!
//! \param ulPWMOut is the output number.
//!
//! This function determines if a PWM output number is valid.
//!
//! \return Returns \b true if the output number is valid and \b false
//! otherwise.
//
//*****************************************************************************
#ifdef DEBUG
static tBoolean
PWMOutValid(unsigned long ulPWMOut)
{
    return((ulPWMOut == PWM_OUT_0) || (ulPWMOut == PWM_OUT_1) ||
           (ulPWMOut == PWM_OUT_2) || (ulPWMOut == PWM_OUT_3) ||
           (ulPWMOut == PWM_OUT_4) || (ulPWMOut == PWM_OUT_5) ||
           (ulPWMOut == PWM_OUT_6) || (ulPWMOut == PWM_OUT_7));
}
#endif

//*****************************************************************************
//
//! Configures a PWM generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to configure.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulConfig is the configuration for the PWM generator.
//!
//! This function is used to set the mode of operation for a PWM generator.
//! The counting mode, synchronization mode, and debug behavior are all
//! configured.  After configuration, the generator is left in the disabled
//! state.
//!
//! A PWM generator can count in two different modes:  count down mode or count
//! up/down mode.  In count down mode, it will count from a value down to zero,
//! and then reset to the preset value.  This will produce left-aligned PWM
//! signals (that is the rising edge of the two PWM signals produced by the
//! generator will occur at the same time).  In count up/down mode, it will
//! count up from zero to the preset value, count back down to zero, and then
//! repeat the process.  This will produce center-aligned PWM signals (that is,
//! the middle of the high/low period of the PWM signals produced by the
//! generator will occur at the same time).
//!
//! When the PWM generator parameters (period and pulse width) are modified,
//! their affect on the output PWM signals can be delayed.  In synchronous
//! mode, the parameter updates are not applied until a synchronization event
//! occurs.  This allows multiple parameters to be modified and take affect
//! simultaneously, instead of one at a time.  Additionally, parameters to
//! multiple PWM generators in synchronous mode can be updated simultaneously,
//! allowing them to be treated as if they were a unified generator.  In
//! non-synchronous mode, the parameter updates are not delayed until a
//! synchronization event.  In either mode, the parameter updates only occur
//! when the counter is at zero to help prevent oddly formed PWM signals during
//! the update (that is, a PWM pulse that is too short or too long).
//!
//! The PWM generator can either pause or continue running when the processor
//! is stopped via the debugger.  If configured to pause, it will continue to
//! count until it reaches zero, at which point it will pause until the
//! processor is restarted.  If configured to continue running, it will keep
//! counting as if nothing had happened.
//!
//! The \e ulConfig parameter contains the desired configuration.  It is the
//! logical OR of the following:
//!
//! - \b PWM_GEN_MODE_DOWN or \b PWM_GEN_MODE_UP_DOWN to specify the counting
//!   mode
//! - \b PWM_GEN_MODE_SYNC or \b PWM_GEN_MODE_NO_SYNC to specify the counter
//!   load and comparator update synchronization mode
//! - \b PWM_GEN_MODE_DBG_RUN or \b PWM_GEN_MODE_DBG_STOP to specify the debug
//!   behavior
//! - \b PWM_GEN_MODE_GEN_NO_SYNC, \b PWM_GEN_MODE_GEN_SYNC_LOCAL, or
//!   \b PWM_GEN_MODE_GEN_SYNC_GLOBAL to specify the update synchronization
//!   mode for generator counting mode changes
//! - \b PWM_GEN_MODE_DB_NO_SYNC, \b PWM_GEN_MODE_DB_SYNC_LOCAL, or
//!   \b PWM_GEN_MODE_DB_SYNC_GLOBAL to specify the deadband parameter
//!   synchronization mode
//! - \b PWM_GEN_MODE_FAULT_LATCHED or \b PWM_GEN_MODE_FAULT_UNLATCHED to
//!   specify whether fault conditions are latched or not
//! - \b PWM_GEN_MODE_FAULT_MINPER or \b PWM_GEN_MODE_FAULT_NO_MINPER to
//!   specify whether minimum fault period support is required
//! - \b PWM_GEN_MODE_FAULT_EXT or \b PWM_GEN_MODE_FAULT_LEGACY to specify
//!   whether extended fault source selection support is enabled or not
//!
//! Setting \b PWM_GEN_MODE_FAULT_MINPER allows an application to set the
//! minimum duration of a PWM fault signal.  Fault will be signaled for at
//! least this time even if the external fault pin deasserts earlier.  Care
//! should be taken when using this mode since during the fault signal period,
//! the fault interrupt from the PWM generator will remain asserted.  The fault
//! interrupt handler may, therefore, reenter immediately if it exits prior to
//! expiration of the fault timer.
//!
//! \note Changes to the counter mode will affect the period of the PWM signals
//! produced.  PWMGenPeriodSet() and PWMPulseWidthSet() should be called after
//! any changes to the counter mode of a generator.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenConfigure(unsigned long ulBase, unsigned long ulGen,
                unsigned long ulConfig)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Compute the generator's base address.
    //
    ulGen = PWM_GEN_BADDR(ulBase, ulGen);

    //
    // Change the global configuration of the generator.
    //
    HWREG(ulGen + PWM_O_X_CTL) = ((HWREG(ulGen + PWM_O_X_CTL) &
                                   ~(PWM_X_CTL_MODE | PWM_X_CTL_DEBUG |
                                     PWM_X_CTL_LATCH | PWM_X_CTL_MINFLTPER |
                                     PWM_X_CTL_FLTSRC | PWM_X_CTL_DBFALLUPD_M |
                                     PWM_X_CTL_DBRISEUPD_M |
                                     PWM_X_CTL_DBCTLUPD_M |
                                     PWM_X_CTL_GENBUPD_M |
                                     PWM_X_CTL_GENAUPD_M |
                                     PWM_X_CTL_LOADUPD | PWM_X_CTL_CMPAUPD |
                                     PWM_X_CTL_CMPBUPD)) | ulConfig);

    //
    // Set the individual PWM generator controls.
    //
    if(ulConfig & PWM_X_CTL_MODE)
    {
        //
        // In up/down count mode, set the signal high on up count comparison
        // and low on down count comparison (that is, center align the
        // signals).
        //
        HWREG(ulGen + PWM_O_X_GENA) = (PWM_X_GENA_ACTCMPAU_ONE |
                                       PWM_X_GENA_ACTCMPAD_ZERO);
        HWREG(ulGen + PWM_O_X_GENB) = (PWM_X_GENB_ACTCMPBU_ONE |
                                       PWM_X_GENB_ACTCMPBD_ZERO);
    }
    else
    {
        //
        // In down count mode, set the signal high on load and low on count
        // comparison (that is, left align the signals).
        //
        HWREG(ulGen + PWM_O_X_GENA) = (PWM_X_GENA_ACTLOAD_ONE |
                                       PWM_X_GENA_ACTCMPAD_ZERO);
        HWREG(ulGen + PWM_O_X_GENB) = (PWM_X_GENB_ACTLOAD_ONE |
                                       PWM_X_GENB_ACTCMPBD_ZERO);
    }
}

//*****************************************************************************
//
//! Set the period of a PWM generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to be modified.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulPeriod specifies the period of PWM generator output, measured
//! in clock ticks.
//!
//! This function sets the period of the specified PWM generator block, where
//! the period of the generator block is defined as the number of PWM clock
//! ticks between pulses on the generator block zero signal.
//!
//! \note Any subsequent calls made to this function before an update occurs
//! will cause the previous values to be overwritten.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenPeriodSet(unsigned long ulBase, unsigned long ulGen,
                unsigned long ulPeriod)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Compute the generator's base address.
    //
    ulGen = PWM_GEN_BADDR(ulBase, ulGen);

    //
    // Set the reload register based on the mode.
    //
    if(HWREG(ulGen + PWM_O_X_CTL) & PWM_X_CTL_MODE)
    {
        //
        // In up/down count mode, set the reload register to half the requested
        // period.
        //
        ASSERT((ulPeriod / 2) < 65536);
        HWREG(ulGen + PWM_O_X_LOAD) = ulPeriod / 2;
    }
    else
    {
        //
        // In down count mode, set the reload register to the requested period
        // minus one.
        //
        ASSERT((ulPeriod <= 65536) && (ulPeriod != 0));
        HWREG(ulGen + PWM_O_X_LOAD) = ulPeriod - 1;
    }
}

//*****************************************************************************
//
//! Gets the period of a PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to query.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//!
//! This function gets the period of the specified PWM generator block.  The
//! period of the generator block is defined as the number of PWM clock ticks
//! between pulses on the generator block zero signal.
//!
//! If the update of the counter for the specified PWM generator has yet
//! to be completed, the value returned may not be the active period.  The
//! value returned is the programmed period, measured in PWM clock ticks.
//!
//! \return Returns the programmed period of the specified generator block
//! in PWM clock ticks.
//
//*****************************************************************************
unsigned long
PWMGenPeriodGet(unsigned long ulBase, unsigned long ulGen)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Compute the generator's base address.
    //
    ulGen = PWM_GEN_BADDR(ulBase, ulGen);

    //
    // Figure out the counter mode.
    //
    if(HWREG(ulGen + PWM_O_X_CTL) & PWM_X_CTL_MODE)
    {
        //
        // The period is twice the reload register value.
        //
        return(HWREG(ulGen + PWM_O_X_LOAD) * 2);
    }
    else
    {
        //
        // The period is the reload register value plus one.
        //
        return(HWREG(ulGen + PWM_O_X_LOAD) + 1);
    }
}

//*****************************************************************************
//
//! Enables the timer/counter for a PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to be enabled.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//!
//! This function allows the PWM clock to drive the timer/counter for the
//! specified generator block.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenEnable(unsigned long ulBase, unsigned long ulGen)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Enable the PWM generator.
    //
    HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_CTL) |= PWM_X_CTL_ENABLE;
}

//*****************************************************************************
//
//! Disables the timer/counter for a PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to be disabled.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//!
//! This function blocks the PWM clock from driving the timer/counter for the
//! specified generator block.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenDisable(unsigned long ulBase, unsigned long ulGen)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Disable the PWM generator.
    //
    HWREG(PWM_GEN_BADDR(ulBase, + ulGen) + PWM_O_X_CTL) &= ~(PWM_X_CTL_ENABLE);
}

//*****************************************************************************
//
//! Sets the pulse width for the specified PWM output.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulPWMOut is the PWM output to modify.  Must be one of \b PWM_OUT_0,
//! \b PWM_OUT_1, \b PWM_OUT_2, \b PWM_OUT_3, \b PWM_OUT_4, \b PWM_OUT_5,
//! \b PWM_OUT_6, or \b PWM_OUT_7.
//! \param ulWidth specifies the width of the positive portion of the pulse.
//!
//! This function sets the pulse width for the specified PWM output, where the
//! pulse width is defined as the number of PWM clock ticks.
//!
//! \note Any subsequent calls made to this function before an update occurs
//! will cause the previous values to be overwritten.
//!
//! \return None.
//
//*****************************************************************************
void
PWMPulseWidthSet(unsigned long ulBase, unsigned long ulPWMOut,
                 unsigned long ulWidth)
{
    unsigned long ulGenBase, ulReg;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMOutValid(ulPWMOut));

    //
    // Compute the generator's base address.
    //
    ulGenBase = PWM_OUT_BADDR(ulBase, ulPWMOut);

    //
    // If the counter is in up/down count mode, divide the width by two.
    //
    if(HWREG(ulGenBase + PWM_O_X_CTL) & PWM_X_CTL_MODE)
    {
        ulWidth /= 2;
    }

    //
    // Get the period.
    //
    ulReg = HWREG(ulGenBase + PWM_O_X_LOAD);

    //
    // Make sure the width is not too large.
    //
    ASSERT(ulWidth < ulReg);

    //
    // Compute the compare value.
    //
    ulReg = ulReg - ulWidth;

    //
    // Write to the appropriate registers.
    //
    if(PWM_IS_OUTPUT_ODD(ulPWMOut))
    {
        HWREG(ulGenBase + PWM_O_X_CMPB) = ulReg;
    }
    else
    {
        HWREG(ulGenBase + PWM_O_X_CMPA) = ulReg;
    }
}

//*****************************************************************************
//
//! Gets the pulse width of a PWM output.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulPWMOut is the PWM output to query.  Must be one of \b PWM_OUT_0,
//! \b PWM_OUT_1, \b PWM_OUT_2, \b PWM_OUT_3, \b PWM_OUT_4, \b PWM_OUT_5,
//! \b PWM_OUT_6, or \b PWM_OUT_7.
//!
//! This function gets the currently programmed pulse width for the specified
//! PWM output.  If the update of the comparator for the specified output has
//! yet to be completed, the value returned may not be the active pulse width.
//! The value returned is the programmed pulse width, measured in PWM clock
//! ticks.
//!
//! \return Returns the width of the pulse in PWM clock ticks.
//
//*****************************************************************************
unsigned long
PWMPulseWidthGet(unsigned long ulBase, unsigned long ulPWMOut)
{
    unsigned long ulGenBase, ulReg, ulLoad;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMOutValid(ulPWMOut));

    //
    // Compute the generator's base address.
    //
    ulGenBase = PWM_OUT_BADDR(ulBase, ulPWMOut);

    //
    // Then compute the pulse width.  If mode is UpDown, set
    // width = (load - compare) * 2.  Otherwise, set width = load - compare.
    //
    ulLoad = HWREG(ulGenBase + PWM_O_X_LOAD);
    if(PWM_IS_OUTPUT_ODD(ulPWMOut))
    {
        ulReg = HWREG(ulGenBase + PWM_O_X_CMPB);
    }
    else
    {
        ulReg = HWREG(ulGenBase + PWM_O_X_CMPA);
    }
    ulReg = ulLoad - ulReg;

    //
    // If in up/down count mode, double the pulse width.
    //
    if(HWREG(ulGenBase + PWM_O_X_CTL) & PWM_X_CTL_MODE)
    {
        ulReg = ulReg * 2;
    }

    //
    // Return the pulse width.
    //
    return(ulReg);
}

//*****************************************************************************
//
//! Enables the PWM dead band output, and sets the dead band delays.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to modify.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param usRise specifies the width of delay from the rising edge.
//! \param usFall specifies the width of delay from the falling edge.
//!
//! This function sets the dead bands for the specified PWM generator,
//! where the dead bands are defined as the number of \b PWM clock ticks
//! from the rising or falling edge of the generator's \b OutA signal.
//! Note that this function causes the coupling of \b OutB to \b OutA.
//!
//! \return None.
//
//*****************************************************************************
void
PWMDeadBandEnable(unsigned long ulBase, unsigned long ulGen,
                  unsigned short usRise, unsigned short usFall)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT(usRise < 4096);
    ASSERT(usFall < 4096);

    //
    // Compute the generator's base address.
    //
    ulGen = PWM_GEN_BADDR(ulBase, ulGen);

    //
    // Write the dead band delay values.
    //
    HWREG(ulGen + PWM_O_X_DBRISE) = usRise;
    HWREG(ulGen + PWM_O_X_DBFALL) = usFall;

    //
    // Enable the deadband functionality.
    //
    HWREG(ulGen + PWM_O_X_DBCTL) |= PWM_X_DBCTL_ENABLE;
}

//*****************************************************************************
//
//! Disables the PWM dead band output.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to modify.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//!
//! This function disables the dead band mode for the specified PWM generator.
//! Doing so decouples the \b OutA and \b OutB signals.
//!
//! \return None.
//
//*****************************************************************************
void
PWMDeadBandDisable(unsigned long ulBase, unsigned long ulGen)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Disable the deadband functionality.
    //
    HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_DBCTL) &=
        ~(PWM_X_DBCTL_ENABLE);
}

//*****************************************************************************
//
//! Synchronizes all pending updates.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGenBits are the PWM generator blocks to be updated.  Must be the
//! logical OR of any of \b PWM_GEN_0_BIT, \b PWM_GEN_1_BIT,
//! \b PWM_GEN_2_BIT, or \b PWM_GEN_3_BIT.
//!
//! For the selected PWM generators, this function causes all queued updates to
//! the period or pulse width to be applied the next time the corresponding
//! counter becomes zero.
//!
//! \return None.
//
//*****************************************************************************
void
PWMSyncUpdate(unsigned long ulBase, unsigned long ulGenBits)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(!(ulGenBits & ~(PWM_GEN_0_BIT | PWM_GEN_1_BIT | PWM_GEN_2_BIT |
                           PWM_GEN_3_BIT)));

    //
    // Synchronize pending PWM register changes.
    //
    HWREG(ulBase + PWM_O_CTL) = ulGenBits;
}

//*****************************************************************************
//
//! Synchronizes the counters in one or multiple PWM generator blocks.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGenBits are the PWM generator blocks to be synchronized.  Must be
//! the logical OR of any of \b PWM_GEN_0_BIT, \b PWM_GEN_1_BIT,
//! \b PWM_GEN_2_BIT, or \b PWM_GEN_3_BIT.
//!
//! For the selected PWM module, this function synchronizes the time base
//! of the generator blocks by causing the specified generator counters to be
//! reset to zero.
//!
//! \return None.
//
//*****************************************************************************
void
PWMSyncTimeBase(unsigned long ulBase, unsigned long ulGenBits)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(!(ulGenBits & ~(PWM_GEN_0_BIT | PWM_GEN_1_BIT | PWM_GEN_2_BIT |
                           PWM_GEN_3_BIT)));

    //
    // Synchronize the counters in the specified generators by writing to the
    // module's synchronization register.
    //
    HWREG(ulBase + PWM_O_SYNC) = ulGenBits;
}

//*****************************************************************************
//
//! Enables or disables PWM outputs.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulPWMOutBits are the PWM outputs to be modified.  Must be the
//! logical OR of any of \b PWM_OUT_0_BIT, \b PWM_OUT_1_BIT, \b PWM_OUT_2_BIT,
//! \b PWM_OUT_3_BIT, \b PWM_OUT_4_BIT, \b PWM_OUT_5_BIT, \b PWM_OUT_6_BIT,
//! or \b PWM_OUT_7_BIT.
//! \param bEnable determines if the signal is enabled or disabled.
//!
//! This function is used to enable or disable the selected PWM outputs.  The
//! outputs are selected using the parameter \e ulPWMOutBits.  The parameter
//! \e bEnable determines the state of the selected outputs.  If \e bEnable is
//! \b true, then the selected PWM outputs are enabled, or placed in the active
//! state.  If \e bEnable is \b false, then the selected outputs are disabled,
//! or placed in the inactive state.
//!
//! \return None.
//
//*****************************************************************************
void
PWMOutputState(unsigned long ulBase, unsigned long ulPWMOutBits,
               tBoolean bEnable)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(!(ulPWMOutBits & ~(PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT |
                              PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT |
                              PWM_OUT_6_BIT | PWM_OUT_7_BIT)));

    //
    // Read the module's ENABLE output control register, and set or clear the
    // requested bits.
    //
    if(bEnable == true)
    {
        HWREG(ulBase + PWM_O_ENABLE) |= ulPWMOutBits;
    }
    else
    {
        HWREG(ulBase + PWM_O_ENABLE) &= ~(ulPWMOutBits);
    }
}

//*****************************************************************************
//
//! Selects the inversion mode for PWM outputs.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulPWMOutBits are the PWM outputs to be modified.  Must be the
//! logical OR of any of \b PWM_OUT_0_BIT, \b PWM_OUT_1_BIT, \b PWM_OUT_2_BIT,
//! \b PWM_OUT_3_BIT, \b PWM_OUT_4_BIT, \b PWM_OUT_5_BIT, \b PWM_OUT_6_BIT, or
//! \b PWM_OUT_7_BIT.
//! \param bInvert determines if the signal is inverted or passed through.
//!
//! This function is used to select the inversion mode for the selected PWM
//! outputs.  The outputs are selected using the parameter \e ulPWMOutBits.
//! The parameter \e bInvert determines the inversion mode for the selected
//! outputs.  If \e bInvert is \b true, this function will cause the specified
//! PWM output signals to be inverted, or made active low.  If \e bInvert is
//! \b false, the specified output will be passed through as is, or be made
//! active high.
//!
//! \return None.
//
//*****************************************************************************
void
PWMOutputInvert(unsigned long ulBase, unsigned long ulPWMOutBits,
                tBoolean bInvert)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(!(ulPWMOutBits & ~(PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT |
                              PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT |
                              PWM_OUT_6_BIT | PWM_OUT_7_BIT)));

    //
    // Read the module's INVERT output control register, and set or clear the
    // requested bits.
    //
    if(bInvert == true)
    {
        HWREG(ulBase + PWM_O_INVERT) |= ulPWMOutBits;
    }
    else
    {
        HWREG(ulBase + PWM_O_INVERT) &= ~(ulPWMOutBits);
    }
}

//*****************************************************************************
//
//! Specifies the level of PWM outputs suppressed in response to a fault
//! condition.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulPWMOutBits are the PWM outputs to be modified.  Must be the
//! logical OR of any of \b PWM_OUT_0_BIT, \b PWM_OUT_1_BIT, \b PWM_OUT_2_BIT,
//! \b PWM_OUT_3_BIT, \b PWM_OUT_4_BIT, \b PWM_OUT_5_BIT, \b PWM_OUT_6_BIT, or
//! \b PWM_OUT_7_BIT.
//! \param bDriveHigh determines if the signal is driven high or low during an
//! active fault condition.
//!
//! This function determines whether a PWM output pin that is suppressed in
//! response to a fault condition will be driven high or low.  The affected
//! outputs are selected using the parameter \e ulPWMOutBits.  The parameter
//! \e bDriveHigh determines the output level for the pins identified by
//! \e ulPWMOutBits.  If \e bDriveHigh is \b true then the selected outputs
//! will be driven high when a fault is detected.  If it is \e false, the pins
//! will be driven low.
//!
//! In a fault condition, pins which have not been configured to be suppressed
//! via a call to PWMOutputFault() are unaffected by this function.
//!
//! \note This function is available only on devices which support extended
//! PWM fault handling.
//!
//! \return None.
//
//*****************************************************************************
void
PWMOutputFaultLevel(unsigned long ulBase, unsigned long ulPWMOutBits,
                    tBoolean bDriveHigh)
{
    //
    // Check the arguments.
    //
    ASSERT(HWREG(SYSCTL_DC5) & SYSCTL_DC5_PWMEFLT);
    ASSERT(ulBase == PWM_BASE);
    ASSERT(!(ulPWMOutBits & ~(PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT |
                              PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT |
                              PWM_OUT_6_BIT | PWM_OUT_7_BIT)));

    //
    // Read the module's FAULT output control register, and set or clear the
    // requested bits.
    //
    if(bDriveHigh == true)
    {
        HWREG(ulBase + PWM_O_FAULTVAL) |= ulPWMOutBits;
    }
    else
    {
        HWREG(ulBase + PWM_O_FAULTVAL) &= ~(ulPWMOutBits);
    }
}

//*****************************************************************************
//
//! Specifies the state of PWM outputs in response to a fault condition.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulPWMOutBits are the PWM outputs to be modified.  Must be the
//! logical OR of any of \b PWM_OUT_0_BIT, \b PWM_OUT_1_BIT, \b PWM_OUT_2_BIT,
//! \b PWM_OUT_3_BIT, \b PWM_OUT_4_BIT, \b PWM_OUT_5_BIT, \b PWM_OUT_6_BIT, or
//! \b PWM_OUT_7_BIT.
//! \param bFaultSuppress determines if the signal is suppressed or passed
//! through during an active fault condition.
//!
//! This function sets the fault handling characteristics of the selected PWM
//! outputs.  The outputs are selected using the parameter \e ulPWMOutBits.
//! The parameter \e bFaultSuppress determines the fault handling
//! characteristics for the selected outputs.  If \e bFaultSuppress is \b true,
//! then the selected outputs will be made inactive.  If \e bFaultSuppress is
//! \b false, then the selected outputs are unaffected by the detected fault.
//!
//! On devices supporting extended PWM fault handling, the state the affected
//! output pins are driven to can be configured with PWMOutputFaultLevel().  If
//! not configured, or if the device does not support extended PWM fault
//! handling, affected outputs will be driven low on a fault condition.
//!
//! \return None.
//
//*****************************************************************************
void
PWMOutputFault(unsigned long ulBase, unsigned long ulPWMOutBits,
               tBoolean bFaultSuppress)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(!(ulPWMOutBits & ~(PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT |
                              PWM_OUT_3_BIT | PWM_OUT_4_BIT | PWM_OUT_5_BIT |
                              PWM_OUT_6_BIT | PWM_OUT_7_BIT)));

    //
    // Read the module's FAULT output control register, and set or clear the
    // requested bits.
    //
    if(bFaultSuppress == true)
    {
        HWREG(ulBase + PWM_O_FAULT) |= ulPWMOutBits;
    }
    else
    {
        HWREG(ulBase + PWM_O_FAULT) &= ~(ulPWMOutBits);
    }
}

//*****************************************************************************
//
//! Registers an interrupt handler for the specified PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator in question.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param pfnIntHandler is a pointer to the function to be called when the PWM
//! generator interrupt occurs.
//!
//! This function will ensure that the interrupt handler specified by
//! \e pfnIntHandler is called when an interrupt is detected for the specified
//! PWM generator block.  This function will also enable the corresponding
//! PWM generator interrupt in the interrupt controller; individual generator
//! interrupts and interrupt sources must be enabled with PWMIntEnable() and
//! PWMGenIntTrigEnable().
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenIntRegister(unsigned long ulBase, unsigned long ulGen,
                  void (*pfnIntHandler)(void))
{
    unsigned long ulInt;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Get the interrupt number associated with the specified generator.
    //
    if(ulGen == PWM_GEN_3)
    {
        ulInt = INT_PWM3;
    }
    else
    {
        ulInt = INT_PWM0 + (ulGen >> 6) - 1;
    }

    //
    // Register the interrupt handler.
    //
    IntRegister(ulInt, pfnIntHandler);

    //
    // Enable the PWMx interrupt.
    //
    IntEnable(ulInt);
}

//*****************************************************************************
//
//! Removes an interrupt handler for the specified PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator in question.  Must be one of
//! \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//!
//! This function will unregister the interrupt handler for the specified
//! PWM generator block.  This function will also disable the corresponding
//! PWM generator interrupt in the interrupt controller; individual generator
//! interrupts and interrupt sources must be disabled with PWMIntDisable() and
//! PWMGenIntTrigDisable().
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenIntUnregister(unsigned long ulBase, unsigned long ulGen)
{
    unsigned long ulInt;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Get the interrupt number associated with the specified generator.
    //
    if(ulGen == PWM_GEN_3)
    {
        ulInt = INT_PWM3;
    }
    else
    {
        ulInt = INT_PWM0 + (ulGen >> 6) - 1;
    }

    //
    // Disable the PWMx interrupt.
    //
    IntDisable(ulInt);

    //
    // Unregister the interrupt handler.
    //
    IntUnregister(ulInt);
}

//*****************************************************************************
//
//! Registers an interrupt handler for a fault condition detected in a PWM
//! module.
//!
//! \param ulBase is the base address of the PWM module.
//! \param pfnIntHandler is a pointer to the function to be called when the PWM
//! fault interrupt occurs.
//!
//! This function will ensure that the interrupt handler specified by
//! \e pfnIntHandler is called when a fault interrupt is detected for the
//! selected PWM module.  This function will also enable the PWM fault
//! interrupt in the NVIC; the PWM fault interrupt must also be enabled at the
//! module level using PWMIntEnable().
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
PWMFaultIntRegister(unsigned long ulBase, void (*pfnIntHandler)(void))
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);

    //
    // Register the interrupt handler, returning an error if one occurs.
    //
    IntRegister(INT_PWM_FAULT, pfnIntHandler);

    //
    // Enable the PWM fault interrupt.
    //
    IntEnable(INT_PWM_FAULT);
}

//*****************************************************************************
//
//! Removes the PWM fault condition interrupt handler.
//!
//! \param ulBase is the base address of the PWM module.
//!
//! This function will remove the interrupt handler for a PWM fault interrupt
//! from the selected PWM module.  This function will also disable the PWM
//! fault interrupt in the NVIC; the PWM fault interrupt must also be disabled
//! at the module level using PWMIntDisable().
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
PWMFaultIntUnregister(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);

    //
    // Disable the PWM fault interrupt.
    //
    IntDisable(INT_PWM_FAULT);

    //
    // Unregister the interrupt handler, returning an error if one occurs.
    //
    IntUnregister(INT_PWM_FAULT);
}

//*****************************************************************************
//
//! Enables interrupts and triggers for the specified PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to have interrupts and triggers enabled.
//! Must be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulIntTrig specifies the interrupts and triggers to be enabled.
//!
//! Unmasks the specified interrupt(s) and trigger(s) by setting the
//! specified bits of the interrupt/trigger enable register for the specified
//! PWM generator.  The \e ulIntTrig parameter is the logical OR of
//! \b PWM_INT_CNT_ZERO, \b PWM_INT_CNT_LOAD, \b PWM_INT_CNT_AU,
//! \b PWM_INT_CNT_AD, \b PWM_INT_CNT_BU, \b PWM_INT_CNT_BD,
//! \b PWM_TR_CNT_ZERO, \b PWM_TR_CNT_LOAD, \b PWM_TR_CNT_AU, \b PWM_TR_CNT_AD,
//! \b PWM_TR_CNT_BU, or \b PWM_TR_CNT_BD.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenIntTrigEnable(unsigned long ulBase, unsigned long ulGen,
                    unsigned long ulIntTrig)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulIntTrig & ~(PWM_INT_CNT_ZERO | PWM_INT_CNT_LOAD |
                          PWM_INT_CNT_AU | PWM_INT_CNT_AD | PWM_INT_CNT_BU |
                          PWM_INT_CNT_BD | PWM_TR_CNT_ZERO | PWM_TR_CNT_LOAD |
                          PWM_TR_CNT_AU | PWM_TR_CNT_AD | PWM_TR_CNT_BU |
                          PWM_TR_CNT_BD)) == 0);

    //
    // Enable the specified interrupts/triggers.
    //
    HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_INTEN) |= ulIntTrig;
}

//*****************************************************************************
//
//! Disables interrupts for the specified PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to have interrupts and triggers disabled.
//! Must be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulIntTrig specifies the interrupts and triggers to be disabled.
//!
//! Masks the specified interrupt(s) and trigger(s) by clearing the
//! specified bits of the interrupt/trigger enable register for the specified
//! PWM generator.  The \e ulIntTrig parameter is the logical OR of
//! \b PWM_INT_CNT_ZERO, \b PWM_INT_CNT_LOAD, \b PWM_INT_CNT_AU,
//! \b PWM_INT_CNT_AD, \b PWM_INT_CNT_BU, \b PWM_INT_CNT_BD,
//! \b PWM_TR_CNT_ZERO, \b PWM_TR_CNT_LOAD, \b PWM_TR_CNT_AU, \b PWM_TR_CNT_AD,
//! \b PWM_TR_CNT_BU, or \b PWM_TR_CNT_BD.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenIntTrigDisable(unsigned long ulBase, unsigned long ulGen,
                     unsigned long ulIntTrig)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulIntTrig & ~(PWM_INT_CNT_ZERO | PWM_INT_CNT_LOAD |
                          PWM_INT_CNT_AU | PWM_INT_CNT_AD | PWM_INT_CNT_BU |
                          PWM_INT_CNT_BD | PWM_TR_CNT_ZERO | PWM_TR_CNT_LOAD |
                          PWM_TR_CNT_AU | PWM_TR_CNT_AD | PWM_TR_CNT_BU |
                          PWM_TR_CNT_BD)) == 0);

    //
    // Disable the specified interrupts/triggers.
    //
    HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_INTEN) &= ~(ulIntTrig);
}

//*****************************************************************************
//
//! Gets interrupt status for the specified PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to query.  Must be one of \b PWM_GEN_0,
//! \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param bMasked specifies whether masked or raw interrupt status is
//! returned.
//!
//! If \e bMasked is set as \b true, then the masked interrupt status is
//! returned; otherwise, the raw interrupt status will be returned.
//!
//! \return Returns the contents of the interrupt status register, or the
//! contents of the raw interrupt status register, for the specified
//! PWM generator.
//
//*****************************************************************************
unsigned long
PWMGenIntStatus(unsigned long ulBase, unsigned long ulGen, tBoolean bMasked)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));

    //
    // Compute the generator's base address.
    //
    ulGen = PWM_GEN_BADDR(ulBase, ulGen);

    //
    // Read and return the specified generator's raw or enabled interrupt
    // status.
    //
    if(bMasked == true)
    {
        return(HWREG(ulGen + PWM_O_X_ISC));
    }
    else
    {
        return(HWREG(ulGen + PWM_O_X_RIS));
    }
}

//*****************************************************************************
//
//! Clears the specified interrupt(s) for the specified PWM generator block.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator to query.  Must be one of \b PWM_GEN_0,
//! \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulInts specifies the interrupts to be cleared.
//!
//! Clears the specified interrupt(s) by writing a 1 to the specified bits
//! of the interrupt status register for the specified PWM generator.  The
//! \e ulInts parameter is the logical OR of \b PWM_INT_CNT_ZERO,
//! \b PWM_INT_CNT_LOAD, \b PWM_INT_CNT_AU, \b PWM_INT_CNT_AD,
//! \b PWM_INT_CNT_BU, or \b PWM_INT_CNT_BD.
//!
//! \note Since there is a write buffer in the Cortex-M3 processor, it may take
//! several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (since NVIC still sees the interrupt source
//! asserted).
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenIntClear(unsigned long ulBase, unsigned long ulGen, unsigned long ulInts)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulInts & ~(PWM_INT_CNT_ZERO | PWM_INT_CNT_LOAD | PWM_INT_CNT_AU |
                       PWM_INT_CNT_AD | PWM_INT_CNT_BU | PWM_INT_CNT_BD)) ==
           0);

    //
    // Clear the requested interrupts by writing ones to the specified bit
    // of the module's interrupt enable register.
    //
    HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_ISC) = ulInts;
}

//*****************************************************************************
//
//! Enables generator and fault interrupts for a PWM module.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGenFault contains the interrupts to be enabled.  Must be a logical
//! OR of any of \b PWM_INT_GEN_0, \b PWM_INT_GEN_1, \b PWM_INT_GEN_2,
//! \b PWM_INT_GEN_3, \b PWM_INT_FAULT0, \b PWM_INT_FAULT1, \b PWM_INT_FAULT2,
//! or \b PWM_INT_FAULT3.
//!
//! Unmasks the specified interrupt(s) by setting the specified bits of
//! the interrupt enable register for the selected PWM module.
//!
//! \return None.
//
//*****************************************************************************
void
PWMIntEnable(unsigned long ulBase, unsigned long ulGenFault)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT((ulGenFault & ~(PWM_INT_GEN_0 | PWM_INT_GEN_1 | PWM_INT_GEN_2 |
                           PWM_INT_GEN_3 | PWM_INT_FAULT0 | PWM_INT_FAULT1 |
                           PWM_INT_FAULT2 | PWM_INT_FAULT3)) == 0);

    //
    // Read the module's interrupt enable register, and enable interrupts
    // for the specified PWM generators.
    //
    HWREG(ulBase + PWM_O_INTEN) |= ulGenFault;
}

//*****************************************************************************
//
//! Disables generator and fault interrupts for a PWM module.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGenFault contains the interrupts to be disabled.  Must be a
//! logical OR of any of \b PWM_INT_GEN_0, \b PWM_INT_GEN_1, \b PWM_INT_GEN_2,
//! \b PWM_INT_GEN_3, \b PWM_INT_FAULT0, \b PWM_INT_FAULT1, \b PWM_INT_FAULT2,
//! or \b PWM_INT_FAULT3.
//!
//! Masks the specified interrupt(s) by clearing the specified bits of
//! the interrupt enable register for the selected PWM module.
//!
//! \return None.
//
//*****************************************************************************
void
PWMIntDisable(unsigned long ulBase, unsigned long ulGenFault)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT((ulGenFault & ~(PWM_INT_GEN_0 | PWM_INT_GEN_1 | PWM_INT_GEN_2 |
                           PWM_INT_GEN_3 | PWM_INT_FAULT0 | PWM_INT_FAULT1 |
                           PWM_INT_FAULT2 | PWM_INT_FAULT3)) == 0);

    //
    // Read the module's interrupt enable register, and disable interrupts
    // for the specified PWM generators.
    //
    HWREG(ulBase + PWM_O_INTEN) &= ~(ulGenFault);
}

//*****************************************************************************
//
//! Clears the fault interrupt for a PWM module.
//!
//! \param ulBase is the base address of the PWM module.
//!
//! Clears the fault interrupt by writing to the appropriate bit of the
//! interrupt status register for the selected PWM module.
//!
//! This function clears only the FAULT0 interrupt and is retained for
//! backwards compatibility.  It is recommended that PWMFaultIntClearExt() be
//! used instead since it supports all fault interrupts supported on devices
//! with and without extended PWM fault handling support.
//!
//! \note Since there is a write buffer in the Cortex-M3 processor, it may take
//! several clock cycles before the interrupt source is actually cleared.
//! Therefore, it is recommended that the interrupt source be cleared early in
//! the interrupt handler (as opposed to the very last action) to avoid
//! returning from the interrupt handler before the interrupt source is
//! actually cleared.  Failure to do so may result in the interrupt handler
//! being immediately reentered (since NVIC still sees the interrupt source
//! asserted).
//!
//! \return None.
//
//*****************************************************************************
void
PWMFaultIntClear(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);

    //
    // Write the only writeable bit in the module's interrupt register.
    //
    HWREG(ulBase + PWM_O_ISC) = PWM_ISC_INTFAULT0;
}

//*****************************************************************************
//
//! Gets the interrupt status for a PWM module.
//!
//! \param ulBase is the base address of the PWM module.
//! \param bMasked specifies whether masked or raw interrupt status is
//! returned.
//!
//! If \e bMasked is set as \b true, then the masked interrupt status is
//! returned; otherwise, the raw interrupt status will be returned.
//!
//! \return The current interrupt status, enumerated as a bit field of
//! \b PWM_INT_GEN_0, \b PWM_INT_GEN_1, \b PWM_INT_GEN_2, \b PWM_INT_GEN_3,
//! \b PWM_INT_FAULT0, \b PWM_INT_FAULT1, \b PWM_INT_FAULT2, and
//! \b PWM_INT_FAULT3.
//!
//*****************************************************************************
unsigned long
PWMIntStatus(unsigned long ulBase, tBoolean bMasked)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);

    //
    // Read and return either the module's raw or enabled interrupt status.
    //
    if(bMasked == true)
    {
        return(HWREG(ulBase + PWM_O_ISC));
    }
    else
    {
        return(HWREG(ulBase + PWM_O_RIS));
    }
}

//*****************************************************************************
//
//! Clears the fault interrupt for a PWM module.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulFaultInts specifies the fault interrupts to clear.
//!
//! Clears one or more fault interrupts by writing to the appropriate bit of
//! the PWM interrupt status register.  The parameter \e ulFaultInts must be
//! the logical OR of any of \b PWM_INT_FAULT0, \b PWM_INT_FAULT1,
//! \b PWM_INT_FAULT2, or \b PWM_INT_FAULT3.
//!
//! When running on a device supporting extended PWM fault handling, the fault
//! interrupts are derived by performing a logical OR of each of the configured
//! fault trigger signals for a given generator.  Therefore, these interrupts
//! are not directly related to the four possible FAULTn inputs to the device
//! but indicate that a fault has been signaled to one of the four possible PWM
//! generators.  On a device without extended PWM fault handling, the interrupt
//! is directly related to the state of the single FAULT pin.
//!
//! \note Since there is a write buffer in the Cortex-M3 processor, it may take
//! several cycles before the interrupt source is actually cleared.  Therefore,
//! it is recommended that the interrupt source be cleared early in the
//! interrupt handler (as opposed to the very last action) to avoid returning
//! from the interrupt handler before the interrupt source is actually cleared.
//! Failure to do so may result in the interrupt handler being immediately
//! reentered (since NVIC still sees the interrupt source asserted).
//!
//! \return None.
//
//*****************************************************************************
void
PWMFaultIntClearExt(unsigned long ulBase, unsigned long ulFaultInts)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == PWM_BASE);
    ASSERT((ulFaultInts & ~(PWM_INT_FAULT0 | PWM_INT_FAULT1 |
                            PWM_INT_FAULT2 | PWM_INT_FAULT3)) == 0);

    //
    // Clear the supplied fault bits.
    //
    HWREG(ulBase + PWM_O_ISC) = ulFaultInts;
}

//*****************************************************************************
//
//! Configures the minimum fault period and fault pin senses for a given
//! PWM generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator whose fault configuration is being set.
//! Must be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulMinFaultPeriod is the minimum fault active period expressed in
//! PWM clock cycles.
//! \param ulFaultSenses indicates which sense of each FAULT input should be
//! considered the ``asserted'' state.  Valid values are logical OR
//! combinations of \b PWM_FAULTn_SENSE_HIGH and \b PWM_FAULTn_SENSE_LOW.
//!
//! This function sets the minimum fault period for a given generator along
//! with the sense of each of the 4 possible fault inputs.  The minimum fault
//! period is expressed in PWM clock cycles and takes effect only if
//! PWMGenConfigure() is called with flag \b PWM_GEN_MODE_FAULT_PER set in the
//! \e ulConfig parameter.  When a fault input is asserted, the minimum fault
//! period timer ensures that it remains asserted for at least the number of
//! clock cycles specified.
//!
//! \note This function is only available on devices supporting extended PWM
//! fault handling.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenFaultConfigure(unsigned long ulBase, unsigned long ulGen,
                     unsigned long ulMinFaultPeriod,
                     unsigned long ulFaultSenses)
{
    //
    // Check the arguments.
    //
    ASSERT(HWREG(SYSCTL_DC5) & SYSCTL_DC5_PWMEFLT);
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT(ulMinFaultPeriod < PWM_X_MINFLTPER_M);
    ASSERT((ulFaultSenses & ~(PWM_FAULT0_SENSE_HIGH | PWM_FAULT0_SENSE_LOW |
                              PWM_FAULT1_SENSE_HIGH | PWM_FAULT1_SENSE_LOW |
                              PWM_FAULT2_SENSE_HIGH | PWM_FAULT2_SENSE_LOW |
                              PWM_FAULT3_SENSE_HIGH | PWM_FAULT3_SENSE_LOW)) ==
           0);

    //
    // Write the minimum fault period.
    //
    HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_MINFLTPER) = ulMinFaultPeriod;

    //
    // Write the fault senses.
    //
    HWREG(PWM_GEN_EXT_BADDR(ulBase, ulGen) + PWM_O_X_FLTSEN) = ulFaultSenses;
}

//*****************************************************************************
//
//! Configures the set of fault triggers for a given PWM generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator whose fault triggers are being set.  Must
//! be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulGroup indicates the subset of possible faults that are to be
//! configured.  This must be \b PWM_FAULT_GROUP_0.
//! \param ulFaultTriggers defines the set of inputs that are to contribute
//! towards generation of the fault signal to the given PWM generator.  For
//! \b PWM_FAULT_GROUP_0, this will be the logical OR of \b PWM_FAULT_FAULT0,
//! \b PWM_FAULT_FAULT1, \b PWM_FAULT_FAULT2, or \b PWM_FAULT_FAULT3.
//!
//! This function allows selection of the set of fault inputs that will be
//! combined to generate a fault condition to a given PWM generator.  By
//! default, all generators use only FAULT0 (for backwards compatibility) but
//! if PWMGenConfigure() is called with flag \b PWM_GEN_MODE_FAULT_SRC in the
//! \e ulConfig parameter, extended fault handling is enabled and this function
//! must be called to configure the fault triggers.
//!
//! The fault signal to the PWM generator is generated by ORing together each
//! of the signals whose inputs are specified in the \e ulFaultTriggers
//! parameter after having adjusted the sense of each FAULTn input based on the
//! configuration previously set using a call to PWMGenFaultConfigure().
//!
//! \note This function is only available on devices supporting extended PWM
//! fault handling.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenFaultTriggerSet(unsigned long ulBase, unsigned long ulGen,
                      unsigned long ulGroup, unsigned long ulFaultTriggers)
{
    //
    // Check for valid parameters.
    //
    ASSERT(HWREG(SYSCTL_DC5) & SYSCTL_DC5_PWMEFLT);
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulGroup == PWM_FAULT_GROUP_0) || (ulGroup == PWM_FAULT_GROUP_1));
    ASSERT((ulFaultTriggers & ~(PWM_FAULT_FAULT0 | PWM_FAULT_FAULT1 |
                                PWM_FAULT_FAULT2 | PWM_FAULT_FAULT3)) == 0);

    //
    // Write the fault triggers to the appropriate register.
    //
    if(ulGroup == PWM_FAULT_GROUP_0)
    {
        HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_FLTSRC0) =
            ulFaultTriggers;
    }
    else
    {
        HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_FLTSRC1) =
            ulFaultTriggers;
    }
}

//*****************************************************************************
//
//! Returns the set of fault triggers currently configured for a given PWM
//! generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator whose fault triggers are being queried.
//! Must be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or \b PWM_GEN_3.
//! \param ulGroup indicates the subset of faults that are being queried.  This
//! must be \b PWM_FAULT_GROUP_0.
//!
//! This function allows an application to query the current set of inputs that
//! contribute towards the generation of a fault condition to a given PWM
//! generator.
//!
//! \note This function is only available on devices supporting extended PWM
//! fault handling.
//!
//! \return Returns the current fault triggers configured for the fault group
//! provided.  For \b PWM_FAULT_GROUP_0, the returned value will be a logical
//! OR of \b PWM_FAULT_FAULT0, \b PWM_FAULT_FAULT1, \b PWM_FAULT_FAULT2, or
//! \b PWM_FAULT_FAULT3.
//
//*****************************************************************************
unsigned long
PWMGenFaultTriggerGet(unsigned long ulBase, unsigned long ulGen,
                      unsigned long ulGroup)
{
    //
    // Check for valid parameters.
    //
    ASSERT(HWREG(SYSCTL_DC5) & SYSCTL_DC5_PWMEFLT);
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulGroup == PWM_FAULT_GROUP_0) || (ulGroup == PWM_FAULT_GROUP_1));

    //
    // Return the current fault triggers.
    //
    if(ulGroup == PWM_FAULT_GROUP_0)
    {
        return(HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_FLTSRC0));
    }
    else
    {
        return(HWREG(PWM_GEN_BADDR(ulBase, ulGen) + PWM_O_X_FLTSRC1));
    }
}

//*****************************************************************************
//
//! Returns the current state of the fault triggers for a given PWM generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator whose fault trigger states are being
//! queried.  Must be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or
//! \b PWM_GEN_3.
//! \param ulGroup indicates the subset of faults that are being queried.  This
//! must be \b PWM_FAULT_GROUP_0.
//!
//! This function allows an application to query the current state of each of
//! the fault trigger inputs to a given PWM generator.  The current state of
//! each fault trigger input is returned unless PWMGenConfigure() has
//! previously been called with flag \b PWM_GEN_MODE_LATCH_FAULT in the
//! \e ulConfig parameter in which case the returned status is the latched
//! fault trigger status.
//!
//! If latched faults are configured, the application must call
//! PWMGenFaultClear() to clear each trigger.
//!
//! \note This function is only available on devices supporting extended PWM
//! fault handling.
//!
//! \return Returns the current state of the fault triggers for the given PWM
//! generator.  A set bit indicates that the associated trigger is active.  For
//! \b PWM_FAULT_GROUP_0, the returned value will be a logical OR of
//! \b PWM_FAULT_FAULT0, \b PWM_FAULT_FAULT1, \b PWM_FAULT_FAULT2, or
//! \b PWM_FAULT_FAULT3.
//
//*****************************************************************************
unsigned long
PWMGenFaultStatus(unsigned long ulBase, unsigned long ulGen,
                  unsigned long ulGroup)
{
    //
    // Check for valid parameters.
    //
    ASSERT(HWREG(SYSCTL_DC5) & SYSCTL_DC5_PWMEFLT);
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulGroup == PWM_FAULT_GROUP_0) || (ulGroup == PWM_FAULT_GROUP_1));

    //
    // Return the current fault status.
    //
    if(ulGroup == PWM_FAULT_GROUP_0)
    {
        return(HWREG(PWM_GEN_EXT_BADDR(ulBase, ulGen) + PWM_O_X_FLTSTAT0));
    }
    else
    {
        return(HWREG(PWM_GEN_EXT_BADDR(ulBase, ulGen) + PWM_O_X_FLTSTAT1));
    }
}

//*****************************************************************************
//
//! Clears one or more latched fault triggers for a given PWM generator.
//!
//! \param ulBase is the base address of the PWM module.
//! \param ulGen is the PWM generator whose fault trigger states are being
//! queried.  Must be one of \b PWM_GEN_0, \b PWM_GEN_1, \b PWM_GEN_2, or
//! \b PWM_GEN_3.
//! \param ulGroup indicates the subset of faults that are being queried.  This
//! must be \b PWM_FAULT_GROUP_0.
//! \param ulFaultTriggers is the set of fault triggers which are to be
//! cleared.
//!
//! This function allows an application to clear the fault triggers for a given
//! PWM generator.  This is only required if PWMGenConfigure() has previously
//! been called with flag \b PWM_GEN_MODE_LATCH_FAULT in parameter \e ulConfig.
//!
//! \note This function is only available on devices supporting extended PWM
//! fault handling.
//!
//! \return None.
//
//*****************************************************************************
void
PWMGenFaultClear(unsigned long ulBase, unsigned long ulGen,
                 unsigned long ulGroup, unsigned long ulFaultTriggers)
{
    //
    // Check for valid parameters.
    //
    ASSERT(HWREG(SYSCTL_DC5) & SYSCTL_DC5_PWMEFLT);
    ASSERT(ulBase == PWM_BASE);
    ASSERT(PWMGenValid(ulGen));
    ASSERT((ulGroup == PWM_FAULT_GROUP_0) || (ulGroup == PWM_FAULT_GROUP_1));
    ASSERT((ulFaultTriggers & ~(PWM_FAULT_FAULT0 | PWM_FAULT_FAULT1 |
                                PWM_FAULT_FAULT2 | PWM_FAULT_FAULT3)) == 0);

    //
    // Clear the given faults.
    //
    if(ulGroup == PWM_FAULT_GROUP_0)
    {
        HWREG(PWM_GEN_EXT_BADDR(ulBase, ulGen) + PWM_O_X_FLTSTAT0) =
            ulFaultTriggers;
    }
    else
    {
        HWREG(PWM_GEN_EXT_BADDR(ulBase, ulGen) + PWM_O_X_FLTSTAT1) =
            ulFaultTriggers;
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
