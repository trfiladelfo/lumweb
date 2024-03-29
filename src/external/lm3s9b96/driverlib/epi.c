//*****************************************************************************
//
// epi.c - Driver for the EPI module.
//
// Copyright (c) 2008-2009 Luminary Micro, Inc.  All rights reserved.
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

#include "inc/hw_epi.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/epi.h"
#include "driverlib/interrupt.h"

//*****************************************************************************
//
//! \addtogroup epi_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Sets the usage mode of the EPI module.
//!
//! \param ulBase is the EPI module base address.
//! \param ulMode is the usage mode of the EPI module.
//!
//! This functions sets the operating mode of the EPI module.  The parameter
//! \e ulMode must be one of the following:
//!
//! - \b EPI_MODE_NONE - non-moded operation
//! - \b EPI_MODE_SDRAM - use with SDRAM device
//! - \b EPI_MODE_HB8 - use with host-bus 8-bit interface
//! - \b EPI_MODE_DISABLE - disable the EPI module
//!
//! Selection of any of the above modes will enable the EPI module, except
//! for \b EPI_MODE_DISABLE which should be used to disable the module.
//!
//! \return None.
//
//*****************************************************************************
void
EPIModeSet(unsigned long ulBase, unsigned long ulMode)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT((ulMode == EPI_MODE_NONE) ||
           (ulMode == EPI_MODE_SDRAM) ||
           (ulMode == EPI_MODE_HB8) ||
           (ulMode == EPI_MODE_DISABLE));

    //
    // Write the mode word to the register.
    //
    HWREG(ulBase + EPI_O_CFG) = ulMode;
}

//*****************************************************************************
//
//! Sets the clock divider for the EPI module.
//!
//! \param ulBase is the EPI module base address.
//! \param ulDivider is the value of the clock divider to be applied to
//! the external interface (0-65535).
//!
//! This functions sets the clock divider that will be used to determine the
//! clock rate of the external interface.  The value is the number of high
//! and low ticks of the system clock per external bus clock.  A value of 0
//! means that the system clock is used without any reduction.  The system
//! clock will be divided by the value of \e ulDivider multiplied by 2.  A
//! value of 1 gives a divider of 2, and value of 2 gives a divider of 4, a
//! value of 3 gives a divider of 6, and so on.
//!
//! \return None.
//
//*****************************************************************************
void
EPIDividerSet(unsigned long ulBase, unsigned long ulDivider)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);

    //
    // Write the divider value to the register.
    //
    HWREG(ulBase + EPI_O_BAUD) = ulDivider;
}

//*****************************************************************************
//
//! Configures the SDRAM mode of operation.
//!
//! \param ulBase is the EPI module base address.
//! \param ulConfig is the SDRAM interface configuration.
//! \param ulRefresh is the refresh count in core clocks (0-2047).
//!
//! This function is used to configure the SDRAM interface, when the SDRAM
//! mode is chosen with the function EPIModeSet().  The parameter \e ulConfig
//! is the logical OR of several sets of choices:
//!
//! The processor core frequency must be specified with one of the following:
//!
//! - \b EPI_SDRAM_CORE_FREQ_0_15 - core clock is 0 MHz < clk <= 15 MHz
//! - \b EPI_SDRAM_CORE_FREQ_15_30 - core clock is 15 MHz < clk <= 30 MHz
//! - \b EPI_SDRAM_CORE_FREQ_30_50 - core clock is 30 MHz < clk <= 50 MHz
//! - \b EPI_SDRAM_CORE_FREQ_50_100 - core clock is 50 MHz < clk <= 100 MHz
//!
//! The low power mode is specified with one of the following:
//!
//! - \b EPI_SDRAM_LOW_POWER - enter low power, self-refresh state
//! - \b EPI_SDRAM_FULL_POWER - normal operating state
//!
//! The SDRAM device size is specified with one of the following:
//!
//! - \b EPI_SDRAM_SIZE_64MBIT - 64 Mbit device (8 MB)
//! - \b EPI_SDRAM_SIZE_128MBIT - 128 Mbit device (16 MB)
//! - \b EPI_SDRAM_SIZE_256MBIT - 256 Mbit device (32 MB)
//! - \b EPI_SDRAM_SIZE_512MBIT - 512 Mbit device (64 MB)
//!
//! The parameter \e ulRefresh sets the refresh counter in units of core
//! clock ticks.  It is an 11-bit value with a range of 0 - 2047 counts.
//!
//! \return None.
//
//*****************************************************************************
void
EPIConfigSDRAMSet(unsigned long ulBase, unsigned long ulConfig,
                  unsigned long ulRefresh)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulRefresh < 2048);

    //
    // Fill in the refresh count field of the configuration word.
    //
    ulConfig &= ~EPI_SDRAMCFG_RFSH_M;
    ulConfig |= ulRefresh << EPI_SDRAMCFG_RFSH_S;

    //
    // Write the SDRAM configuration register.
    //
    HWREG(ulBase + EPI_O_SDRAMCFG) = ulConfig;
}

//*****************************************************************************
//
//! Configures the interface for Host-bus 8 operation.
//!
//! \param ulBase is the EPI module base address.
//! \param ulConfig is the interface configuration.
//! \param ulMaxWait is the maximum number of external clocks to wait
//! if a FIFO ready signal is holding off the transaction.
//!
//! This function is used to configure the interface when used in Host-bus 8
//! operation as chosen with the function EPIModeSet().  The parameter
//! \e ulConfig is the logical OR of any of the following:
//!
//! - one of \b EPI_HB8_MODE_ADMUX, \b EPI_HB8_MODE_ADDEMUX,
//! \b EPI_HB8_MODE_SRAM, or \b EPI_HB8_MODE_FIFO to select the HB8 mode
//! - \b EPI_HB8_USE_TXEMPTY - enable TXEMPTY signal with FIFO
//! - \b EPI_HB8_USE_RXFULL - enable RXFULL signal with FIFO
//! - \b EPI_HB8_WRHIGH - use active high write strobe, otherwise it is
//! active low
//! - \b EPI_HB8_RDHIGH - use active high read strobe, otherwise it is
//! active low
//! - one of \b EPI_HB8_WRWAIT_0, \b EPI_HB8_WRWAIT_1, \b EPI_HB8_WRWAIT_2,
//! or \b EPI_HB8_WRWAIT_3 to select the number of write wait states (default
//! is 0 wait states)
//! - one of \b EPI_HB8_RDWAIT_0, \b EPI_HB8_RDWAIT_1, \b EPI_HB8_RDWAIT_2,
//! or \b EPI_HB8_RDWAIT_3 to select the number of read wait states (default
//! is 0 wait states)
//! - \b EPI_HB8_WORD_ACCESS - use Word Access mode to route bytes to the
//! correct byte lanes allowing data to be stored in bits [31:8].  If absent,
//! all data transfers use bits [7:0].
//! - \b EPI_HB8_CSCFG_CS - sets EPI30 to operate as a Chip Select (CSn)
//! signal.  When using this mode, \b EPI_HB8_MODE_ADMUX must not be specified.
//! If this option is absent, EPI30 operates as an addres latch enable (ALE)
//! signal.
//!
//! The parameter \e ulMaxWait is used if the FIFO mode is chosen.  If a
//! FIFO is used along with RXFULL or TXEMPTY ready signals, then this
//! parameter determines the maximum number of clocks to wait when the
//! transaction is being held off by by the FIFO using one of these ready
//! signals.  A value of 0 means to wait forever.
//!
//! \return None.
//
//*****************************************************************************
void
EPIConfigHB8Set(unsigned long ulBase, unsigned long ulConfig,
                unsigned long ulMaxWait)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulMaxWait < 256);

    //
    // Determine the CS and word access modes.
    //
    HWREG(ulBase + EPI_O_HB8CFG2) = (((ulConfig & EPI_HB8_WORD_ACCESS) ?
                                       EPI_HB8CFG2_WORD : 0) |
                                     ((ulConfig & EPI_HB8_CSCFG_CS) ?
                                       EPI_HB8CFG2_CSCFG : 0));

    //
    // Fill in the max wait field of the configuration word.
    //
    ulConfig &= ~EPI_HB8CFG_MAXWAIT_M;
    ulConfig |= ulMaxWait << EPI_HB8CFG_MAXWAIT_S;

    //
    // Write the main HostBus8 configuration register.
    //
    HWREG(ulBase + EPI_O_HB8CFG)  = ulConfig;
}

//*****************************************************************************
//
//! Configures the interface for non-moded operation.
//!
//! \param ulBase is the EPI module base address.
//! \param ulConfig is the interface configuration.
//! \param ulFrameCount is the frame size in clocks, if the frame signal
//! is used (0-15).
//! \param ulMaxWait is the maximum number of external clocks to wait
//! when the external clock enable is holding off the transaction (0-255).
//!
//! This function is used to configure the interface when used in non-moded
//! operation as chosen with the function EPIModeSet().  The parameter
//! \e ulConfig is the logical OR of any of the following:
//!
//! - \b EPI_NONMODE_CLKPIN - interface clock is output on a pin
//! - \b EPI_NONMODE_CLKSTOP - clock is stopped when there is no transaction,
//! otherwise it is free-running
//! - \b EPI_NONMODE_CLKENA - enable the clock enable input from the device
//! - \b EPI_NONMODE_FRAMEPIN - framing signal is emitted on a pin
//! - \b EPI_NONMODE_FRAME50 - framing signal is 50/50 duty cycle, otherwise it
//! is a pulse
//! - \b EPI_NONMODE_READWRITE - read and write strobes are emitted on pins
//! - \b EPI_NONMODE_WRITE2CYCLE - a two cycle write is used, otherwise a
//! single-cycle write is used
//! - \b EPI_NONMODE_READ2CYCLE - a two cycle read is used, otherwise a
//! single-cycle read is used
//! - \b EPI_NONMODE_ASIZE_NONE, \b EPI_NONMODE_ASIZE_4,
//! \b EPI_NONMODE_ASIZE_12, or \b EPI_NONMODE_ASIZE_20 to choose no address
//! bus, or and address bus size of 4, 12, or 20 bits
//! - \b EPI_NONMODE_DSIZE_8, \b EPI_NONMODE_DSIZE_16,
//! \b EPI_NONMODE_DSIZE_24, or \b EPI_NONMODE_DSIZE_32 to select a data bus
//! size of 8, 16, 24, or 32 bits
//! - \b EPI_NONMODE_WORD_ACCESS - use Word Access mode to route bytes to the
//! correct byte lanes allowing data to be stored in the upper bits of the word
//! when necessary.
//!
//! The parameter \e ulFrameCount is the number of clocks used to form the
//! framing signal, if the framing signal is used.  The behavior depends on
//! whether the frame signal is a pulse or a 50/50 duty cycle.  This value
//! is not used if the framing signal is not enabled with the option
//! \b EPI_NONMMODE_FRAMEPIN.
//!
//! The parameter \e ulMaxWait is used if the external clock enable is turned
//! on with the \b EPI_NONMODE_CLKENA option is used.  In the case that
//! external clock enable is used, this parameter determines the maximum
//! number of clocks to wait when the external clock enable signal is holding
//! off a transaction.  A value of 0 means to wait forever.  If a non-zero
//! value is used and exceeded, an interrupt will occur and the transaction
//! aborted.
//!
//! \return None.
//
//*****************************************************************************
void
EPIConfigNoModeSet(unsigned long ulBase, unsigned long ulConfig,
                   unsigned long ulFrameCount, unsigned long ulMaxWait)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulFrameCount < 16);
    ASSERT(ulMaxWait < 256);

    //
    // Set the word access mode.
    //
    HWREG(ulBase + EPI_O_GPCFG2) = ((ulConfig & EPI_NONMODE_WORD_ACCESS) ?
                                    EPI_GPCFG2_WORD : 0);

    //
    // Fill in the frame count field of the configuration word.
    //
    ulConfig &= ~EPI_GPCFG_FRMCNT_M;
    ulConfig |= ulFrameCount << EPI_GPCFG_FRMCNT_S;

    //
    // Fill in the max wait field of the configuration word.
    //
    ulConfig &= ~EPI_GPCFG_MAXWAIT_M;
    ulConfig |= ulMaxWait << EPI_GPCFG_MAXWAIT_S;

    //
    // Write the non-moded configuration register.
    //
    HWREG(ulBase + EPI_O_GPCFG) = ulConfig;
}

//*****************************************************************************
//
//! Configures the address map for the external interface.
//!
//! \param ulBase is the EPI module base address.
//! \param ulMap is the address mapping configuration.
//!
//! This function is used to configure the address mapping for the external
//! interface.  This determines the base address of the external memory or
//! device within the processor peripheral and/or memory space.
//!
//! The parameter \e ulMap is the logical OR of the following:
//!
//! - \b EPI_ADDR_PER_SIZE_256B, \b EPI_ADDR_PER_SIZE_64KB,
//! \b EPI_ADDR_PER_SIZE_16MB, or \b EPI_ADDR_PER_SIZE_512MB to choose a
//! peripheral address space of 256 bytes, 64 Kbytes, 16 Mbytes or 512 Mbytes
//! - \b EPI_ADDR_PER_BASE_NONE, \b EPI_ADDR_PER_BASE_A, or
//! \b EPI_ADDR_PER_BASE_C to choose the base address of the peripheral
//! space as none, 0xA0000000, or 0xC0000000
//! - \b EPI_ADDR_RAM_SIZE_256B, \b EPI_ADDR_RAM_SIZE_64KB,
//! \b EPI_ADDR_RAM_SIZE_16MB, or \b EPI_ADDR_RAM_SIZE_512MB to choose a
//! RAM address space of 256 bytes, 64 Kbytes, 16 Mbytes or 512 Mbytes
//! - \b EPI_ADDR_RAM_BASE_NONE, \b EPI_ADDR_RAM_BASE_6, or
//! \b EPI_ADDR_RAM_BASE_8 to choose the base address of the RAM space
//! as none, 0x60000000, or 0x80000000
//!
//! \return None.
//
//*****************************************************************************
void
EPIAddressMapSet(unsigned long ulBase, unsigned long ulMap)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulMap < 0x100);

    //
    // Set the value of the address mapping register.
    //
    HWREG(ulBase + EPI_O_ADDRMAP) = ulMap;
}

//*****************************************************************************
//
//! Configures a non-blocking read transaction.
//!
//! \param ulBase is the EPI module base address.
//! \param ulChannel is the read channel (0 or 1).
//! \param ulDataSize is the size of the data items to read.
//! \param ulAddress is the starting address to read.
//!
//! This function is used to configure a non-blocking read channel for a
//! transaction.  Two channels are available which can be used in a ping-pong
//! method for continuous reading.  It is not necessary to use both channels
//! to perform a non-blocking read.
//!
//! The parameter \e ulDataSize is one of \b EPI_NBCONFIG_SIZE_8,
//! \b EPI_NBCONFIG_SIZE_16, or \b EPI_NBCONFIG_SIZE_32 for 8-bit, 16-bit,
//! or 32-bit sized data transfers.
//!
//! The parameter \e ulAddress is the starting address for the read, relative
//! to the external device.  The start of the device is address 0.
//!
//! Once configured, the non-blocking read is started by calling
//! EPINonBlockingReadStart().  If the addresses to be read from the device
//! are in a sequence, it is not necessary to call this function multiple
//! times.  Until it is changed, the EPI module will remember the last address
//! that was used for a non-blocking read (per channel).
//!
//! \return None.
//
//*****************************************************************************
void
EPINonBlockingReadConfigure(unsigned long ulBase, unsigned long ulChannel,
                            unsigned long ulDataSize, unsigned long ulAddress)
{
    unsigned long ulOffset;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulChannel < 2);
    ASSERT(ulDataSize < 4);
    ASSERT(ulAddress < 0x20000000);

    //
    // Compute the offset needed to select the correct channel regs.
    //
    ulOffset = ulChannel * (EPI_O_RSIZE1 - EPI_O_RSIZE0);

    //
    // Write the data size register for the channel.
    //
    HWREG(ulBase + EPI_O_RSIZE0 + ulOffset) = ulDataSize;

    //
    // Write the starting address register for the channel.
    //
    HWREG(ulBase + EPI_O_RADDR0 + ulOffset) = ulAddress;
}

//*****************************************************************************
//
//! Starts a non-blocking read transaction.
//!
//! \param ulBase is the EPI module base address.
//! \param ulChannel is the read channel (0 or 1).
//! \param ulCount is the number of items to read (1-4095).
//!
//! This function starts a non-blocking read that was previously configured
//! with the function EPINonBlockingReadConfigure().  Once this function is
//! called, the EPI module will begin reading data from the external device
//! into the read FIFO.  The EPI will stop reading when the FIFO fills up
//! and resume reading when the application drains the FIFO, until the
//! total specified count of data items has been read.
//!
//! Once a read transaction is completed and the FIFO drained, another
//! transaction can be started from the next address by calling this
//! function again.
//!
//! \return None.
//
//*****************************************************************************
void
EPINonBlockingReadStart(unsigned long ulBase, unsigned long ulChannel,
                        unsigned long ulCount)
{
    unsigned long ulOffset;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulChannel < 2);
    ASSERT(ulCount < 4096);

    //
    // Compute the offset needed to select the correct channel regs.
    //
    ulOffset = ulChannel * (EPI_O_RPSTD1 - EPI_O_RPSTD0);

    //
    // Write to the read count register.
    //
    HWREG(ulBase + EPI_O_RPSTD0 + ulOffset) = ulCount;
}

//*****************************************************************************
//
//! Stops a non-blocking read transaction.
//!
//! \param ulBase is the EPI module base address.
//! \param ulChannel is the read channel (0 or 1).
//!
//! This function cancels a non-blocking read transaction that is already
//! in progress.
//!
//! \return None.
//
//*****************************************************************************
void
EPINonBlockingReadStop(unsigned long ulBase, unsigned long ulChannel)
{
    unsigned long ulOffset;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulChannel < 2);

    //
    // Compute the offset needed to select the correct channel regs.
    //
    ulOffset = ulChannel * (EPI_O_RPSTD1 - EPI_O_RPSTD0);

    //
    // Write a 0 to the read count register, which will cancel the transaction.
    //
    HWREG(ulBase + EPI_O_RPSTD0 + ulOffset) = 0;
}

//*****************************************************************************
//
//! Get the count remaining for a non-blocking transaction.
//!
//! \param ulBase is the EPI module base address.
//! \param ulChannel is the read channel (0 or 1).
//!
//! This function gets the remaining count of items for a non-blocking read
//! transaction.
//!
//! \return The number of items remaining in the non-blocking read transaction.
//
//*****************************************************************************
unsigned long
EPINonBlockingReadCount(unsigned long ulBase, unsigned long ulChannel)
{
    unsigned long ulOffset;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulChannel < 2);

    //
    // Compute the offset needed to select the correct channel regs.
    //
    ulOffset = ulChannel * (EPI_O_RPSTD1 - EPI_O_RPSTD0);

    //
    // Read the count remaining and return the value to the caller.
    //
    return(HWREG(ulBase + EPI_O_RPSTD0 + ulOffset));
}

//*****************************************************************************
//
//! Get the count of items available in the read FIFO.
//!
//! \param ulBase is the EPI module base address.
//!
//! This function gets the number of items that are available to read in
//! the read FIFO.  The read FIFO is filled by a non-blocking read transaction
//! which is configured by the functions EPINonBlockingReadConfigure() and
//! EPINonBlockingReadStart().
//!
//! \return The number of items available to read in the read FIFO.
//
//*****************************************************************************
unsigned long
EPINonBlockingReadAvail(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);

    //
    // Read the FIFO count and return it to the caller.
    //
    return(HWREG(ulBase + EPI_O_RFIFOCNT));
}

//*****************************************************************************
//
//! Read available data from the read FIFO, as 32-bit data items.
//!
//! \param ulBase is the EPI module base address.
//! \param ulCount is the maximum count of items to read.
//! \param pulBuf is the caller supplied buffer where the read data should
//! be stored.
//!
//! This function reads 32-bit data items from the read FIFO and stores
//! the values in a caller supplied buffer.  The function will read and store
//! data from the FIFO until there is no more data in the FIFO or the maximum
//! count is reached as specified in the parameter \e ulCount.  The actual
//! count of items will be returned.
//!
//! \return The number of items read from the FIFO.
//
//*****************************************************************************
unsigned long
EPINonBlockingReadGet32(unsigned long ulBase, unsigned long ulCount,
                        unsigned long *pulBuf)
{
    unsigned long ulCountRead = 0;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulCount < 4096);
    ASSERT(pulBuf);

    //
    // Read from the FIFO while there are any items to read, and
    // the callers specified count is not exceeded.
    //
    while(HWREG(ulBase + EPI_O_RFIFOCNT) && ulCount--)
    {
        //
        // Read from the FIFO and store in the caller supplied buffer.
        //
        *pulBuf = HWREG(ulBase + EPI_O_READFIFO);

        //
        // Update the caller's buffer pointer and the count of items read.
        //
        pulBuf++;
        ulCountRead++;
    }

    //
    // Return the count of items read to the caller.
    //
    return(ulCountRead);
}

//*****************************************************************************
//
//! Read available data from the read FIFO, as 16-bit data items.
//!
//! \param ulBase is the EPI module base address.
//! \param ulCount is the maximum count of items to read.
//! \param pusBuf is the caller supplied buffer where the read data should
//! be stored.
//!
//! This function reads 16-bit data items from the read FIFO and stores
//! the values in a caller supplied buffer.  The function will read and store
//! data from the FIFO until there is no more data in the FIFO or the maximum
//! count is reached as specified in the parameter \e ulCount.  The actual
//! count of items will be returned.
//!
//! \return The number of items read from the FIFO.
//
//*****************************************************************************
unsigned long
EPINonBlockingReadGet16(unsigned long ulBase, unsigned long ulCount,
                        unsigned short *pusBuf)
{
    unsigned long ulCountRead = 0;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulCount < 4096);
    ASSERT(pusBuf);

    //
    // Read from the FIFO while there are any items to read, and
    // the callers specified count is not exceeded.
    //
    while(HWREG(ulBase + EPI_O_RFIFOCNT) && ulCount--)
    {
        //
        // Read from the FIFO and store in the caller supplied buffer.
        //
        *pusBuf = (unsigned short)HWREG(ulBase + EPI_O_READFIFO);

        //
        // Update the caller's buffer pointer and the count of items read.
        //
        pusBuf++;
        ulCountRead++;
    }

    //
    // Return the count of items read to the caller.
    //
    return(ulCountRead);
}

//*****************************************************************************
//
//! Read available data from the read FIFO, as 8-bit data items.
//!
//! \param ulBase is the EPI module base address.
//! \param ulCount is the maximum count of items to read.
//! \param pucBuf is the caller supplied buffer where the read data should
//! be stored.
//!
//! This function reads 8-bit data items from the read FIFO and stores
//! the values in a caller supplied buffer.  The function will read and store
//! data from the FIFO until there is no more data in the FIFO or the maximum
//! count is reached as specified in the parameter \e ulCount.  The actual
//! count of items will be returned.
//!
//! \return The number of items read from the FIFO.
//
//*****************************************************************************
unsigned long
EPINonBlockingReadGet8(unsigned long ulBase, unsigned long ulCount,
                       unsigned char *pucBuf)
{
    unsigned long ulCountRead = 0;

    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulCount < 4096);
    ASSERT(pucBuf);

    //
    // Read from the FIFO while there are any items to read, and
    // the callers specified count is not exceeded.
    //
    while(HWREG(ulBase + EPI_O_RFIFOCNT) && ulCount--)
    {
        //
        // Read from the FIFO and store in the caller supplied buffer.
        //
        *pucBuf = (unsigned char)HWREG(ulBase + EPI_O_READFIFO);

        //
        // Update the caller's buffer pointer and the count of items read.
        //
        pucBuf++;
        ulCountRead++;
    }

    //
    // Return the count of items read to the caller.
    //
    return(ulCountRead);
}

//*****************************************************************************
//
//! Configures the read FIFO.
//!
//! \param ulBase is the EPI module base address.
//! \param ulConfig is the FIFO configuration.
//!
//! This function configures the FIFO trigger levels and error
//! generation.  The parameter \e ulConfig is the logical OR of the
//! following:
//!
//! - \b EPI_FIFO_CONFIG_WTFULLERR - enables an error interrupt when a write is
//! attempted and the write FIFO is full
//! - \b EPI_FIFO_CONFIG_RSTALLERR - enables an error interrupt when a read is
//! stalled due to an interleaved write or other reason
//! - \b EPI_FIFO_CONFIG_TX_EMPTY, \b EPI_FIFO_CONFIG_TX_1_4,
//! \b EPI_FIFO_CONFIG_TX_1_2, or \b EPI_FIFO_CONFIG_TX_3_4 to set the
//! TX FIFO trigger level to empty, 1/4, 1/2, or 3/4 level
//! - \b EPI_FIFO_CONFIG_RX_1_8, \b EPI_FIFO_CONFIG_RX_1_4,
//! \b EPI_FIFO_CONFIG_RX_1_2, \b EPI_FIFO_CONFIG_RX_3_4,
//! \b EPI_FIFO_CONFIG_RX_7_8, or \b EPI_FIFO_CONFIG_RX_FULL to set the
//! RX FIFO trigger level to 1/8, 1/4, 1/2, 3/4, 7/8 or full level
//!
//! \return None.
//
//*****************************************************************************
void
EPIFIFOConfig(unsigned long ulBase, unsigned long ulConfig)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulConfig == (ulConfig & 0x00030077));

    //
    // Load the configuration into the FIFO config reg.
    //
    HWREG(ulBase + EPI_O_FIFOLVL) = ulConfig;
}

//*****************************************************************************
//
//! Reads the number of empty slots in the write transaction FIFO.
//!
//! \param ulBase is the EPI module base address.
//!
//! This function returns the number of slots available in the transaction
//! FIFO.  It can be used in a polling method to avoid attempting a write
//! that would stall.
//!
//! \return The number of empty slots in the transaction FIFO.
//
//*****************************************************************************
unsigned long
EPINonBlockingWriteCount(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);

    //
    // Read the FIFO count and return it to the caller.
    //
    return(HWREG(ulBase + EPI_O_WFIFOCNT));
}

//*****************************************************************************
//
//! Enables EPI interrupt sources.
//!
//! \param ulBase is the EPI module base address.
//! \param ulIntFlags is a bit mask of the interrupt sources to be enabled.
//!
//! This function enables the specified EPI sources to generate interrupts.
//! The \e ulIntFlags parameter can be the logical OR of any of the following
//! values:
//!
//! - \b EPI_INT_TXREQ - transmit FIFO is below the trigger level
//! - \b EPI_INT_RXREQ - read FIFO is above the trigger level
//! - \b EPI_INT_ERR - an error condition occurred
//!
//! \return Returns None.
//
//*****************************************************************************
void
EPIIntEnable(unsigned long ulBase, unsigned long ulIntFlags)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulIntFlags < 16);

    //
    // Write the interrupt flags mask to the mask register.
    //
    HWREG(ulBase + EPI_O_IM) |= ulIntFlags;
}

//*****************************************************************************
//
//! Disables EPI interrupt sources.
//!
//! \param ulBase is the EPI module base address.
//! \param ulIntFlags is a bit mask of the interrupt sources to be disabled.
//!
//! This function disables the specified EPI sources for interrupt
//! generation.  The \e ulIntFlags parameter can be the logical OR
//! of any of the following values: \b EPI_INT_RXREQ, \b EPI_INT_TXREQ, or
//! \b I2S_INT_ERR.
//!
//! \return Returns None.
//
//*****************************************************************************
void
EPIIntDisable(unsigned long ulBase, unsigned long ulIntFlags)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulIntFlags < 16);

    //
    // Write the interrupt flags mask to the mask register.
    //
    HWREG(ulBase + EPI_O_IM) &= ~ulIntFlags;
}

//*****************************************************************************
//
//! Gets the EPI interrupt status.
//!
//! \param ulBase is the EPI module base address.
//! \param bMasked is set \b true to get the masked interrupt status, or
//! \b false to get the raw interrupt status.
//!
//! This function returns the EPI interrupt status.  It can return either
//! the raw or masked interrupt status.
//!
//! \return Returns the masked or raw EPI interrupt status, as a bit field
//! of any of the following values: \b EPI_INT_TXREQ, \b EPI_INT_RXREQ,
//! or \b EPI_INT_ERR
//
//*****************************************************************************
unsigned long
EPIIntStatus(unsigned long ulBase, tBoolean bMasked)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);

    //
    // Return either the interrupt status or the raw interrupt status as
    // requested.
    //
    if(bMasked)
    {
        return(HWREG(ulBase + EPI_O_MIS));
    }
    else
    {
        return(HWREG(ulBase + EPI_O_RIS));
    }
}

//*****************************************************************************
//
//! Gets the EPI error interrupt status.
//!
//! \param ulBase is the EPI module base address.
//!
//! This function returns the error status of the EPI.  If the return value of
//! the function EPIIntStatus() has the flag \b EPI_INT_ERR set, then this
//! function can be used to determine the cause of the error.
//!
//! This function returns a bit mask of error flags, which can be the logical
//! OR of any of the following:
//!
//! - \b EPI_INT_ERR_WTFULL - occurs when a write stalled when the transaction
//! FIFO was full
//! - \b EPI_INT_ERR_RSTALL - occurs when a read stalled
//! - \b EPI_INT_ERR_TIMEOUT - occurs when the external clock enable held
//! off a transaction longer than the configured maximum wait time
//!
//! \return Returns the interrupt error flags as the logical OR of any of
//! the following: \b EPI_INT_ERR_WTFULL, \b EPI_INT_ERR_RSTALL, or
//! \b EPI_INT_ERR_TIMEOUT.
//
//*****************************************************************************
unsigned long
EPIIntErrorStatus(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);

    //
    // Read the error status and return to caller.
    //
    return(HWREG(ulBase + EPI_O_EISC));
}

//*****************************************************************************
//
//! Clears pending EPI error sources.
//!
//! \param ulBase is the EPI module base address.
//! \param ulErrFlags is a bit mask of the error sources to be cleared.
//!
//! This function clears the specified pending EPI errors.  The \e ulErrFlags
//! parameter can be the logical OR of any of the following values:
//! \b EPI_INT_ERR_WTFULL, \b EPI_INT_ERR_RSTALL, or \b EPI_INT_ERR_TIMEOUT.
//!
//! \return Returns None.
//
//*****************************************************************************
void
EPIIntErrorClear(unsigned long ulBase, unsigned long ulErrFlags)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(ulErrFlags < 16);

    //
    // Write the error flags to the register to clear the pending errors.
    //
    HWREG(ulBase + EPI_O_EISC) = ulErrFlags;
}

//*****************************************************************************
//
//! Registers an interrupt handler for the EPI module.
//!
//! \param ulBase is the EPI module base address.
//! \param pfnHandler is a pointer to the function to be called when the
//! interrupt is activated.
//!
//! This sets and enables the handler to be called when the EPI module
//! generates an interrupt.  Specific EPI interrupts must still be enabled
//! with the EPIIntEnable() function.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
EPIIntRegister(unsigned long ulBase, void (*pfnHandler)(void))
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);
    ASSERT(pfnHandler);

    //
    // Register the interrupt handler.
    //
    IntRegister(INT_EPI0, pfnHandler);

    //
    // Enable the EPI interface interrupt.
    //
    IntEnable(INT_EPI0);
}

//*****************************************************************************
//
//! Unregisters an interrupt handler for the EPI module.
//!
//! \param ulBase is the EPI module base address.
//!
//! This function will disable and clear the handler to be called when the
//! EPI interrupt occurs.
//!
//! \sa IntRegister() for important information about registering interrupt
//! handlers.
//!
//! \return None.
//
//*****************************************************************************
void
EPIIntUnregister(unsigned long ulBase)
{
    //
    // Check the arguments.
    //
    ASSERT(ulBase == EPI0_BASE);

    //
    // Disable the EPI interface interrupt.
    //
    IntDisable(INT_EPI0);

    //
    // Unregister the interrupt handler.
    //
    IntUnregister(INT_EPI0);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
