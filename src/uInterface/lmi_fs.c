//*****************************************************************************
//
// lmi_fs.c - File System Processing for lwIP Web Server Apps.
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


/**
 * \addtogroup System
 * @{
 *
 * \author Anziner, Hahn
 * \brief Contains the starting point for the application
 *
 */
#include "FreeRTOS.h"
#include <string.h>
#include "lmi_fs.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "ethernet/httpd/fsdata.h"
#include "fatfs/ff.h"
#include "fatfs/diskio.h"

#include "fatfs/mmc.h"

//*****************************************************************************
//
// Include the file system data for this application.  This file is generated
// by the makefsfile utility, using the following command:
//
//     makefsfile -i fs -o lmi-fsdata.h -r -h
//
// If any changes are made to the static content of the web pages served by the
// application, this command must be used to regenerate lmi-fsdata.h in order
// for those changes to be picked up by the web server.
//
//*****************************************************************************
#include "lmi-fsdata.h"

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;
static volatile tBoolean g_bFatFsEnabled = false;

//*****************************************************************************
//
// Enable the SSI Port for FatFs usage.
//
//*****************************************************************************
void fs_enable(unsigned long ulFrequency)
{
	//
	// Disable the SSI Port.
	//
	SSIDisable(SSI0_BASE);

	//
	// Reconfigure the SSI Port for Fat FS operation.
	//
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
			SSI_MODE_MASTER, ulFrequency, 8);

	//
	// Eanble the SSI Port.
	//
	SSIEnable(SSI0_BASE);
}

//*****************************************************************************
//
// Initialize the file system.
//
//*****************************************************************************
void fs_init(void)
{
	FRESULT fresult;
	DIR g_sDirObject;

	//
	// Initialze the flag to indicate that we are disabled.
	//
	g_bFatFsEnabled = false;

	//
	// Initialize and mount the Fat File System.
	//
	fresult = f_mount(0, &g_sFatFs);
	if (fresult != FR_OK)
	{
		return;
	}

	//
	// Open the root directory for access.
	//
	fresult = f_opendir(&g_sDirObject, "/");

	//
	// Flag and display which file system we are using.
	//

	if (fresult == FR_OK)
	{
		//
		// Indicate and display that we are using the SD file system.
		//
		g_bFatFsEnabled = true;
	}
	else
	{
		//
		// Indicate and display that we are using the internal file system.
		//
		g_bFatFsEnabled = false;
	}
}

//*****************************************************************************
// File System tick handler.
//*****************************************************************************
void fs_tick(unsigned long ulTickMS)
{
	static unsigned long ulTickCounter = 0;

	//
	// Check if the file system has been enabled yet.
	//
	if (!g_bFatFsEnabled)
	{
		return;
	}

	//
	// Increment the tick counter.
	//
	ulTickCounter += ulTickMS;

	//
	// Check to see if the FAT FS tick needs to run.
	//
	if (ulTickCounter >= 10)
	{
		ulTickCounter = 0;
		disk_timerproc();
	}
}

//*****************************************************************************
//
// Open a file and return a handle to the file, if found.  Otherwise,
// return NULL.
//
//*****************************************************************************
struct fs_file *
fs_open(char *name)
{
	const struct fsdata_file *ptTree;
	struct fs_file *ptFile = NULL;
	FIL *ptFatFile = NULL;
	FRESULT fresult = FR_OK;

	//
	// Allocate memory for the file system structure.
	//
	ptFile = (struct fs_file *) pvPortMalloc(sizeof(struct fs_file));
	if (NULL == ptFile)
	{
		return (NULL);
	}

	//
	// Check to see if the Fat File System has been enabled.
	//
	if (g_bFatFsEnabled)
	{
		//
		// Ensure that the file system access to the SSI port is active.
		//
		fs_enable(400000);

		//
		// Allocate memory for the Fat File system handle.
		//
		ptFatFile = (FIL*) pvPortMalloc(sizeof(FIL));
		if (NULL == ptFatFile)
		{
			vPortFree(ptFile);
			return (NULL);
		}

		//
		// Attempt to open the file on the Fat File System.
		//
		fresult = f_open(ptFatFile, name, FA_READ);
		if (FR_OK == fresult)
		{
			ptFile->data = NULL;
			ptFile->len = 0;
			ptFile->index = 0;
			ptFile->pextension = ptFatFile;
			return (ptFile);
		}

		//
		// If we get here, we failed to find the file on the Fat File System,
		// so free up the Fat File system handle/object.
		//
		vPortFree(ptFatFile);
		vPortFree(ptFile);
		return (NULL);
	}

	//
	// Initialize the file system tree pointer to the root of the linked list.
	//
	ptTree = FS_ROOT;

	//
	// Begin processing the linked list, looking for the requested file name.
	//
	while (NULL != ptTree)
	{
		//
		// Compare the requested file "name" to the file name in the
		// current node.
		//
		if (strncmp(name, (char *) ptTree->name, ptTree->len) == 0)
		{
			//
			// Fill in the data pointer and length values from the
			// linked list node.
			//
			ptFile->data = (char *) ptTree->data;
			ptFile->len = ptTree->len;

			//
			// For now, we setup the read index to the end of the file,
			// indicating that all data has been read.
			//
			ptFile->index = ptTree->len;

			//
			// We are not using any file system extensions in this
			// application, so set the pointer to NULL.
			//
			ptFile->pextension = NULL;

			//
			// Exit the loop and return the file system pointer.
			//
			break;
		}

		//
		// If we get here, we did not find the file at this node of the linked
		// list.  Get the next element in the list.
		//
		ptTree = ptTree->next;
	}

	//
	// If we didn't find the file, ptTee will be NULL.  Make sure we
	// return a NULL pointer if this happens.
	//
	if (NULL == ptTree)
	{
		vPortFree(ptFile);
		ptFile = NULL;
	}

	//
	// Return the file system pointer.
	//
	return (ptFile);
}

//*****************************************************************************
//
// Close an opened file designated by the handle.
//
//*****************************************************************************
void fs_close(struct fs_file *file)
{
	//
	// If a Fat file was opened, free its object.
	//
	if (file->pextension)
	{
		vPortFree(file->pextension);
	}

	//
	// Free the main file system object.
	//
	vPortFree(file);
}

//*****************************************************************************
//
// Read the next chunck of data from the file.  Return the count of data
// that was read.  Return 0 if no data is currently available.  Return
// a -1 if at the end of file.
//
//*****************************************************************************
int fs_read(struct fs_file *file, char *buffer, int count)
{
	int iAvailable;

	//
	// Check to see if a Fat File was opened and process it.
	//
	if (file->pextension)
	{
		UINT usBytesRead;
		FRESULT fresult;

		//
		// Ensure that the file system access to the SSI port is active.
		//
		fs_enable(400000);

		//
		// Read the data.
		//
		fresult = f_read(file->pextension, buffer, count, &usBytesRead);
		if ((fresult != FR_OK) || (usBytesRead == 0))
		{
			return (-1);
		}
		return ((int) usBytesRead);
	}

	//
	// Check to see if more data is available.
	//
	if (file->len == file->index)
	{
		//
		// There is no remaining data.  Return a -1 for EOF indication.
		//
		return (-1);
	}

	//
	// Determine how much data we can copy.  The minimum of the 'count'
	// parameter or the available data in the file system buffer.
	//
	iAvailable = file->len - file->index;
	if (iAvailable > count)
	{
		iAvailable = count;
	}

	//
	// Copy the data.
	//
	memcpy(buffer, file->data + iAvailable, iAvailable);
	file->index += iAvailable;

	//
	// Return the count of data that we copied.
	//
	return (iAvailable);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

