//*****************************************************************************
//
// ringbuf.c - Ring buffer management utilities.
//
// Copyright (c) 2008 Luminary Micro, Inc.  All rights reserved.
// 
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
// This is part of revision 2692 of the Stellaris Peripheral Driver Library.
//
//*****************************************************************************

#include "hw_types.h"
#include "debug.h"
#include "ringbuf.h"

//*****************************************************************************
//
//! \addtogroup rdk_s2e_ringbuf_api Ring Buffer API Functions
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Define NULL, if not already defined.
//
//*****************************************************************************
#ifndef NULL
#define NULL                    ((void *)0)
#endif

//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is full or not.
//!
//! \param ptRingBuf is the ring buffer object to empty.
//!
//! This function is used to determine whether or not a given ring buffer is
//! full.  The structure is specifically to ensure that we do not see
//! warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is full or \b false otherwise.
//
//*****************************************************************************
tBoolean
RingBufFull(tRingBufObject *ptRingBuf)
{
    unsigned long ulWrite;
    unsigned long ulRead;

    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->ulWriteIndex;
    ulRead = ptRingBuf->ulReadIndex;

    //
    // Return the full status of the buffer.
    //
    return((((ulWrite + 1) % ptRingBuf->ulSize) == ulRead) ? true : false);
}

//*****************************************************************************
//
//! Determines whether the ring buffer whose pointers and size are provided
//! is empty or not.
//!
//! \param ptRingBuf is the ring buffer object to empty.
//!
//! This function is used to determine whether or not a given ring buffer is
//! empty.  The structure is specifically to ensure that we do not see
//! warnings from the compiler related to the order of volatile accesses
//! being undefined.
//!
//! \return Returns \b true if the buffer is empty or \b false otherwise.
//
//*****************************************************************************
tBoolean
RingBufEmpty(tRingBufObject *ptRingBuf)
{
    unsigned long ulWrite;
    unsigned long ulRead;

    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->ulWriteIndex;
    ulRead = ptRingBuf->ulReadIndex;

    //
    // Return the empty status of the buffer.
    //
    return((ulWrite == ulRead) ? true : false);
}

//*****************************************************************************
//
//! Empties the ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to empty.
//!
//! Discards all data from the ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
RingBufFlush(tRingBufObject *ptRingBuf)
{
    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Set the Read/Write pointers to be the same.
    //
    ptRingBuf->ulReadIndex = ptRingBuf->ulWriteIndex;
}

//*****************************************************************************
//
//! Returns number of bytes stored in ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the number of bytes stored in the ring buffer.
//!
//! \return Returns the number of bytes stored in the ring buffer.
//
//*****************************************************************************
unsigned long
RingBufUsed(tRingBufObject *ptRingBuf)
{
    unsigned long ulWrite;
    unsigned long ulRead;

    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Copy the Read/Write indices for calculation.
    //
    ulWrite = ptRingBuf->ulWriteIndex;
    ulRead = ptRingBuf->ulReadIndex;

    //
    // Return the number of bytes contained in the ring buffer.
    //
    return((ulWrite >= ulRead) ? (ulWrite - ulRead) :
           (ptRingBuf->ulSize - (ulRead - ulWrite)));
}

//*****************************************************************************
//
//! Returns number of bytes available in a ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the number of bytes available in the ring buffer.
//!
//! \return Returns the number of bytes available in the ring buffer.
//
//*****************************************************************************
unsigned long
RingBufFree(tRingBufObject *ptRingBuf)
{
    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Return the number of bytes available in the ring buffer.
    //
    return(ptRingBuf->ulSize - 1 - RingBufUsed(ptRingBuf));
}

//*****************************************************************************
//
//! Return size in bytes of a ring buffer.
//!
//! \param ptRingBuf is the ring buffer object to check.
//!
//! This function returns the size of the ring buffer.
//!
//! \return Returns the size in bytes of the ring buffer.
//
//*****************************************************************************
unsigned long
RingBufSize(tRingBufObject *ptRingBuf)
{
    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Return the number of bytes available in the ring buffer.
    //
    return(ptRingBuf->ulSize);
}

//*****************************************************************************
//
//! Reads a single byte of data from a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be written to.
//!
//! This function reads a single byte of data from a ring buffer.
//!
//! \return The byte read from the ring buffer.
//
//*****************************************************************************
unsigned char
RingBufReadOne(tRingBufObject *ptRingBuf)
{
    unsigned char ucTemp;

    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Verify that space is available in the buffer.
    //
    ASSERT(RingBufUsed(ptRingBuf) != 0);

    //
    // Write the data byte.
    //
    ucTemp = ptRingBuf->pucBuf[ptRingBuf->ulReadIndex];

    //
    // Increment the write index.
    //
    ptRingBuf->ulReadIndex = (ptRingBuf->ulReadIndex + 1) % ptRingBuf->ulSize;

    //
    // Return the character read.
    //
    return(ucTemp);
}

//*****************************************************************************
//
//! Reads data from a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be read from.
//! \param pucData points to where the data should be stored.
//! \param ulLength is the number of bytes to be read.
//!
//! This function reads a sequence of bytes from a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
RingBufRead(tRingBufObject *ptRingBuf, unsigned char *pucData,
            unsigned long ulLength)
{
    unsigned long ulTemp;

    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);
    ASSERT(pucData != NULL);
    ASSERT(ulLength != 0);

    //
    // Verify that data is available in the buffer.
    //
    ASSERT(ulLength <= RingBufUsed(ptRingBuf));

    //
    // Read the data from the ring buffer.
    //
    for(ulTemp = 0; ulTemp < ulLength; ulTemp++)
    {
        pucData[ulTemp] = RingBufReadOne(ptRingBuf);
    }
}

//*****************************************************************************
//
//! Writes a single byte of data to a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be written to.
//! \param ucData is the byte to be written.
//!
//! This function writes a single byte of data into a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
RingBufWriteOne(tRingBufObject *ptRingBuf, unsigned char ucData)
{
    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);

    //
    // Verify that space is available in the buffer.
    //
    ASSERT(RingBufFree(ptRingBuf) != 0);

    //
    // Write the data byte.
    //
    ptRingBuf->pucBuf[ptRingBuf->ulWriteIndex] = ucData;

    //
    // Increment the write index.
    //
    ptRingBuf->ulWriteIndex = ((ptRingBuf->ulWriteIndex + 1) %
                               ptRingBuf->ulSize);
}

//*****************************************************************************
//
//! Writes data to a ring buffer.
//!
//! \param ptRingBuf points to the ring buffer to be written to.
//! \param pucData points to the data to be written.
//! \param ulLength is the number of bytes to be written.
//!
//! This function write a sequence of bytes into a ring buffer.
//!
//! \return None.
//
//*****************************************************************************
void
RingBufWrite(tRingBufObject *ptRingBuf, unsigned char *pucData,
             unsigned long ulLength)
{
    unsigned long ulTemp;

    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);
    ASSERT(pucData != NULL);
    ASSERT(ulLength != 0);

    //
    // Verify that space is available in the buffer.
    //
    ASSERT(ulLength <= RingBufFree(ptRingBuf));

    //
    // Write the data into the ring buffer.
    //
    for(ulTemp = 0; ulTemp < ulLength; ulTemp++)
    {
        RingBufWriteOne(ptRingBuf, pucData[ulTemp]);
    }
}

//*****************************************************************************
//
//! Initialize a ring buffer object.
//!
//! \param ptRingBuf points to the ring buffer to be initialized.
//! \param pucBuf points to the data buffer to be used for the ring buffer.
//! \param ulSize is the size of the buffer in bytes.
//!
//! This function initializes a ring buffer object, preparing it to store data.
//!
//! \return None.
//
//*****************************************************************************
void
RingBufInit(tRingBufObject *ptRingBuf, unsigned char *pucBuf,
            unsigned long ulSize)
{
    //
    // Check the arguments.
    //
    ASSERT(ptRingBuf != NULL);
    ASSERT(pucBuf != NULL);
    ASSERT(ulSize != 0);

    //
    // Initialize the ring buffer object.
    //
    ptRingBuf->ulSize = ulSize;
    ptRingBuf->pucBuf = pucBuf;
    ptRingBuf->ulWriteIndex = ptRingBuf->ulReadIndex = 0;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
