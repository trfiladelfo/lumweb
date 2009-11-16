//*****************************************************************************
//
// lmi_fs.c - File System Processing for enet_io application.
//
// Copyright (c) 2007-2009 Luminary Micro, Inc.  All rights reserved.
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
// This is part of revision 4053 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw_types.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "fs.h"
#include "../cgi/io.h"
#include "fatfs/ff.h"

#include "FreeRTOS.h"
#include "task.h"

//*****************************************************************************
//
// Include the file system data for this application.  This file is generated
// by the makefsfile utility, using the following command (all on one line):
//
//     makefsfile -i fs -o io_fsdata.c -r -h
//
// If any changes are made to the static content of the web pages served by the
// application, this script must be used to regenerate io_fsdata.h in order
// for those changes to be picked up by the web server.
//
//*****************************************************************************

#define  TEMP_FILE_SIZE		0x4000
#define  TEMP_FILE				1

extern int http_stats_display(char * pcBuf, int iBufLen);
extern int Cmd_ls(char * buffer, int BufferSize, char * NewPath);
//*****************************************************************************
//
// Open a file and return a handle to the file, if found.  Otherwise,
// return NULL.  This function also looks for special filenames used to
// provide specific status information or to control various subsystems.
// These filenames are used by the JavaScript on the "IO Control Demo 1"
// example web page.
//
//*****************************************************************************
struct fs_file *
fs_open(char *name)
{
    const struct fsdata_file *ptTree;
    struct fs_file *ptFile = NULL;
    FRESULT fResult;
    FIL *file;
    char *path;

    printf("%s \n", name);

    fResult = f_open(file, name+1, FA_READ);
    ptFile = mem_malloc(sizeof(struct fs_file));

	if(fResult == FR_OK){
	    ptFile->data = "gelesen";
	    ptFile->index = 0;
	    ptFile->len = strlen(ptFile->data);
	}else
	{
		    ptFile->data = "404";
		    ptFile->index = 0;
		    ptFile->len = strlen(ptFile->data);

	}
	return ptFile;
    /*    struct fs_file *tmpFile = NULL;
    UINT usBytesRead;

    //
    // Allocate memory for the file system structure.
    //
    ptFile = mem_malloc(sizeof(struct fs_file));
    if(NULL == ptFile)
    {
        return(NULL);
    }

    //
    // Process request for IP statistics
    //
    if(strncmp(name, "/IPstats.html", sizeof("/IPstats.html") - 1) == 0)
    {
        //
        // build temporary file
        //
		    ptFile->data = mem_malloc( TEMP_FILE_SIZE );
		    if(NULL == ptFile->data)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
				memset(ptFile->data, '\0', TEMP_FILE_SIZE);
				ptFile->pextension = (void *)TEMP_FILE; // needed for fs_close
				tmpFile = fs_open("/IPstats.head");
		    if(NULL == tmpFile)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    if(tmpFile->len > TEMP_FILE_SIZE)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    //
		    // Copy the data.
		    //
				ptFile->len = tmpFile->len;
				ptFile->index = 0;
		    memcpy(ptFile->data, tmpFile->data, tmpFile->len);
				fs_close(tmpFile);
		    //
		    // Fill in statistics.
		    //
		*/
#if LWIP_STATS /* don't build if not configured for use in lwipopts.h */
    //    ptFile->len += http_stats_display(ptFile->data + ptFile->len, TEMP_FILE_SIZE - ptFile->len);
#endif /* LWIP_STATS */
/*				tmpFile = fs_open("/IPstats.tail");
		    if(NULL == tmpFile)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    if(tmpFile->len > (TEMP_FILE_SIZE - ptFile->len))
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    //
		    // Copy the data.
		    //
		    memcpy(ptFile->data + ptFile->len, tmpFile->data, tmpFile->len);
				ptFile->len += tmpFile->len;
				fs_close(tmpFile);
        //
        // For now, we setup the read index to the end of the file,
        // indicating that all data has been read.
        //
        ptFile->index = ptFile->len;
        //
        // Return the file system pointer.
        //
        return(ptFile);
    }

    //
    // Process request for Free RTOS statistics
    //
    if(strncmp(name, "/RTOSstats.html", sizeof("/RTOSstats.html") - 1) == 0)
    {
        //
        // build temporary file
        //
		    ptFile->data = mem_malloc( TEMP_FILE_SIZE );
		    if(NULL == ptFile->data)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
				memset(ptFile->data, '\0', TEMP_FILE_SIZE);
				ptFile->pextension = (void *)TEMP_FILE; // needed for fs_close
				tmpFile = fs_open("/RTOSstats.head");
		    if(NULL == tmpFile)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    if(tmpFile->len > TEMP_FILE_SIZE)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    //
		    // Copy the data.
		    //
				ptFile->len = tmpFile->len;
				ptFile->index = 0;
		    memcpy(ptFile->data, tmpFile->data, tmpFile->len);
				fs_close(tmpFile);
		    //
		    // Fill in statistics.
		    //
       // ptFile->len += vTaskList(ptFile->data + ptFile->len, TEMP_FILE_SIZE - ptFile->len);

				tmpFile = fs_open("/RTOSstats.tail");
		    if(NULL == tmpFile)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    if(tmpFile->len > (TEMP_FILE_SIZE - ptFile->len))
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    //
		    // Copy the data.
		    //
		    memcpy(ptFile->data + ptFile->len, tmpFile->data, tmpFile->len);
				ptFile->len += tmpFile->len;
				fs_close(tmpFile);
        //
        // For now, we setup the read index to the end of the file,
        // indicating that all data has been read.
        //
        ptFile->index = ptFile->len;
        //
        // Return the file system pointer.
        //
        return(ptFile);
    }

    //
    // Process request for Micro SD Card Contents
    //
    if(strncmp(name, "/Logging.html", sizeof("/Logging.html") - 1) == 0)
    {
				if (name[strlen(name) - 1] != '/')
				{
			    //
			    // Open the file for reading.
			    // If there was some problem opening the file, then return
			    // an error.
			    //
			    ptFile->pextension = (FIL *)mem_malloc( sizeof(FIL) );
			    if(NULL == ptFile->pextension)
			    {
	            mem_free(ptFile);
			        return(NULL);
			    }
			    if(f_open(ptFile->pextension, strchr(name + 1, '/') + 1, FA_READ) != FR_OK)
			    {
            mem_free(ptFile->pextension);
            mem_free(ptFile);
			      return(NULL);
			    }
			    ptFile->data = mem_malloc( TCP_MSS );
			    if(NULL == ptFile->data)
			    {
              mem_free(ptFile->pextension);
	            mem_free(ptFile);
			        return(NULL);
			    }
					//
	        // Read a block of data from the file.  Read as much as can fit
	        // in the temporary buffer, including a space for the trailing null.
	        // If there was an error reading, then print a newline and
	        // return the error to the user.
	        //
	        if(f_read(ptFile->pextension, ptFile->data, TCP_MSS, &usBytesRead) != FR_OK)
	        {
	            mem_free(ptFile->data);
              mem_free(ptFile->pextension);
	            mem_free(ptFile);
	            return(NULL);
	        }
					if (((FIL *)(ptFile->pextension))->fsize > TCP_MSS)
					{
        		ptFile->len = TCP_MSS;
					  ptFile->index = 0;
					}
					else
					{
        		ptFile->len = ((FIL *)ptFile->pextension)->fsize;
					  ptFile->index = ((FIL *)ptFile->pextension)->fsize;
					}
          return(ptFile);
				}
        //
        // build temporary file
        //
		    ptFile->data = mem_malloc( TEMP_FILE_SIZE );
		    if(NULL == ptFile->data)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
				memset(ptFile->data, '\0', TEMP_FILE_SIZE);
				ptFile->pextension = (void *)TEMP_FILE; // needed for fs_close
				tmpFile = fs_open("/Logging.head");
		    if(NULL == tmpFile)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    if(tmpFile->len > TEMP_FILE_SIZE)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    //
		    // Copy the data.
		    //
				ptFile->len = tmpFile->len;
				ptFile->index = 0;
		    memcpy(ptFile->data, tmpFile->data, tmpFile->len);
				fs_close(tmpFile);
		    //
		    // List Directory.
		    //
        ptFile->len += Cmd_ls(ptFile->data + ptFile->len, TEMP_FILE_SIZE - ptFile->len, name);

				tmpFile = fs_open("/Logging.tail");
		    if(NULL == tmpFile)
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    if(tmpFile->len > (TEMP_FILE_SIZE - ptFile->len))
		    {
            mem_free(ptFile);
		        return(NULL);
		    }
		    //
		    // Copy the data.
		    //
		    memcpy(ptFile->data + ptFile->len, tmpFile->data, tmpFile->len);
				ptFile->len += tmpFile->len;
				fs_close(tmpFile);
        //
        // For now, we setup the read index to the end of the file,
        // indicating that all data has been read.
        //
        ptFile->index = ptFile->len;
        //
        // Return the file system pointer.
        //
        return(ptFile);
    }

    //
    // If I can't find it there, look in the rest of the main file system
    //
    else
    {

    }

    //
    // If we didn't find the file, ptTee will be NULL.  Make sure we
    // return a NULL pointer if this happens.
    //
    if(NULL == ptTree)
    {
        mem_free(ptFile);
        ptFile = NULL;
    }

    //
    // Return the file system pointer.
    //
    return(ptFile);
	*/
}

//*****************************************************************************
//
// Close an opened file designated by the handle.
//
//*****************************************************************************
void
fs_close(struct fs_file *file)
{
    //
    // Check to see if temp mem has been allocated
    //
    if(file->pextension == (void *)TEMP_FILE)
    {
        //
        // Nothting to do for this file type.
        //
         mem_free(file->data);
    }
    //
    // Check to see if SD_CARD_FILE has been allocated
    //
    if(file->pextension > (void *)TEMP_FILE)
    {
		     f_close(file->pextension);
				 mem_free(file->pextension);
         mem_free(file->data);
    }
    //
    // Free the main file system object.
    //
    mem_free(file);
}

//*****************************************************************************
//
// Read the next chunck of data from the file.  Return the count of data
// that was read.  Return 0 if no data is currently available.  Return
// a -1 if at the end of file.
//
//*****************************************************************************
int
fs_read(struct fs_file *file, char *buffer, int count)
{
    int iAvailable;
    UINT usBytesRead;

    //
    // Check to see if more data is available.
    //
    if(file->len == file->index)
    {
        //
        // There is no remaining data.  Return a -1 for EOF indication.
        //
        return(-1);
    }

    //
    // Check to see if SD_CARD_FILE has been allocated
    //
    if(file->pextension > (void *)TEMP_FILE)
    {
				//
		    // Read a block of data from the file.  Read as much as can fit
		    // in the temporary buffer, including a space for the trailing null.
		    // If there was an error reading, then print a newline and
		    // return the error to the user.
		    //
				if (file->index == 0)
				{
        		file->len = ((FIL *)file->pextension)->fsize;
					  file->index = TCP_MSS;
				}
		    if(f_read(file->pextension, buffer, count, &usBytesRead) == FR_OK)
		    {
						file->index += usBytesRead;
		        return(usBytesRead);
		    }
		    return(usBytesRead);
    }

    //
    // Determine how much data we can copy.  The minimum of the 'count'
    // parameter or the available data in the file system buffer.
    //
    iAvailable = file->len - file->index;
    if(iAvailable > count)
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
    return(iAvailable);
}
