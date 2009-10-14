#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"

#include "ff.h"
#include "fs.h"

#include "diskio.h"

xSemaphoreHandle FSSema;

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;

//*****************************************************************************
//! \addtogroup FS_API
//! @{
//*****************************************************************************

//*****************************************************************************
//! Initiate the file system.
//!
//! Initiate the file system, must be called as first.
//!
//! \return None.
//*****************************************************************************
void FSInit(void)
{
	//    FSEnable(10000000);

	f_mount(0, &g_sFatFs);

	vSemaphoreCreateBinary(FSSema);
}

//*****************************************************************************
//! \param[in] name zero terminated string containing the full file path and name.
//!
//! This function open an existing file.
//!
//! \return Returns the the newly created file handle if successful or
//! \b NULL in case of error.
//*****************************************************************************
FILE_HANDLE FSOpen(char *name)
{
	FIL *pFile = NULL;
	FRESULT fresult = FR_OK;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return INVALID_FILE_HANDLE;
	}

	//
	// Ensure that the file system access to the SSI port is active.
	//
	//FSEnable(400000);

	//
	// Allocate memory for the Fat File system handle.
	//
	pFile = (FIL*) pvPortMalloc(sizeof(FIL));
	if (pFile == NULL)
	{
		xSemaphoreGive(FSSema);
		return INVALID_FILE_HANDLE;
	}

	//
	// Attempt to open the file on the Fat File System.
	//
	fresult = f_open(pFile, name, FA_READ | FA_WRITE);
	if (FR_OK == fresult)
	{
		xSemaphoreGive(FSSema);
		return ((FILE_HANDLE) pFile);
	}

	//
	// If we get here, we failed to find the file on the Fat File System,
	//
	vPortFree(pFile);
	xSemaphoreGive(FSSema);
	return INVALID_FILE_HANDLE;
}

//*****************************************************************************
//! \param[in] name zero terminated string containing the full file path and name.
//!
//! This function creates a file.
//!
//! \note If a file with the same name already exist it will be overwritten by a new empty file.
//!
//! \return Returns the the newly created file handle if successful or
//! \b NULL in case of error.
//*****************************************************************************
FILE_HANDLE FSCreate(char *name)
{
	FIL *pFile = NULL;
	FRESULT fresult = FR_OK;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return INVALID_FILE_HANDLE;
	}

	//
	// Ensure that the file system access to the SSI port is active.
	//
	//FSEnable(400000);

	//
	// Allocate memory for the Fat File system handle.
	//
	pFile = (FIL*) pvPortMalloc(sizeof(FIL));
	if (pFile == NULL)
	{
		xSemaphoreGive(FSSema);
		return INVALID_FILE_HANDLE;
	}

	//
	// Attempt to open the file on the Fat File System.
	//
	fresult = f_open(pFile, name, FA_CREATE_ALWAYS | FA_READ | FA_WRITE);
	if (FR_OK == fresult)
	{
		xSemaphoreGive(FSSema);
		return ((FILE_HANDLE*) pFile);
	}

	//
	// If we get here, we failed to find the file on the Fat File System,
	//
	vPortFree(pFile);
	xSemaphoreGive(FSSema);
	return INVALID_FILE_HANDLE;
}

//*****************************************************************************
//! \param[in] handle file handle.
//!
//! After the function \e FSOpen or \e FSCreate has been called successfully,
//! use this API function to close the file and free the memory resources used by the handle.
//!
//! \return None.
//*****************************************************************************
BOOL FSClose(FILE_HANDLE handle)
{
	FRESULT fresult = FR_OK + 1;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return INVALID_FILE_HANDLE;
	}

	//
	// If a Fat file was opened, free its object.
	//
	if (handle)
	{
		fresult = f_close((FIL*) handle);
		vPortFree(handle);
	}

	xSemaphoreGive(FSSema);

	if (fresult == FR_OK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//*****************************************************************************
//! \param[in] handle file handle.
//! \param[in] buffer pointer to the data array to read to.
//! \param[in] count length of the data array in bytes.
//!
//! Use this function to read data from a file.
//!
//! \return Returns the count of data that was read. Return 0 if no data is currently available. Return
//! -1 if at the end of file.
//*****************************************************************************
int FSRead(FILE_HANDLE handle, void *buffer, int count)
{
	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return 0;
	}
	//
	// Check to see if a Fat File was opened and process it.
	//
	if (handle)
	{
		unsigned short usBytesRead;
		FRESULT fresult;

		//
		// Ensure that the file system access to the SSI port is active.
		//
		//FSEnable(400000);

		//
		// Read the data.
		//
		fresult = f_read((FIL*) handle, buffer, count, &usBytesRead);
		if ((fresult != FR_OK) || (usBytesRead == 0))
		{
			xSemaphoreGive(FSSema);
			return 0;
		}
		xSemaphoreGive(FSSema);
		return (int) usBytesRead;
	}
	xSemaphoreGive(FSSema);
	return 0;
}

//*****************************************************************************
//! \param[in] handle file handle.
//! \param[in] buffer pointer to the data array to write from.
//! \param[in] count length of the data array in bytes.
//!
//! Use this function to write data to a file.
//!
//! \return Returns the count of data that was written.
//*****************************************************************************
int FSWrite(FILE_HANDLE handle, void *buffer, int count)
{
	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return 0;
	}
	//
	// Check to see if a Fat File was opened and process it.
	//
	if (handle)
	{
		unsigned short usBytesRead;
		FRESULT fresult;

		//
		// Ensure that the file system access to the SSI port is active.
		//
		//FSEnable(400000);

		//
		// Read the data.
		//
		fresult = f_write((FIL*) handle, buffer, count, &usBytesRead);
		if ((fresult != FR_OK) || (usBytesRead == 0))
		{
			xSemaphoreGive(FSSema);
			return 0;
		}
		xSemaphoreGive(FSSema);
		return (int) usBytesRead;
	}
	xSemaphoreGive(FSSema);
	return 0;
}

//*****************************************************************************
//! \param[in] handle file handle.
//! \param[in] offset value in bytes of the offset to use.
//!
//! This function moves the read pointer forward or backward from the actual value.
//!
//! \return Returns \b TRUE if ok or \b FALSE fails.
//*****************************************************************************
BOOL FSMoveReadPointer(FILE_HANDLE handle, int offset)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	// Shift the pointer
	fresult = f_lseek((FIL*) handle, ((FIL*) handle)->fptr + offset);

	//
	// Check for error and return if there is a problem.
	//
	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	return TRUE;
}

//*****************************************************************************
//! \param[out] pDirObject pointer used to get the directory handle.
//! \param[in] path zero terminated string containing the full directory path.
//!
//! This function opens an exsisting directory and creates the directory used object for subsequent \e FSReadDir calls.
//! The directory object structure can be discarded at any time without any procedure.
//!
//! \return Returns \b TRUE if ok or \b FALSE fails.
//*****************************************************************************
BOOL FSOpenDir(DIR_OBJECT *pDirObject, char *path)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	//
	// Open the current directory for access.
	//
	fresult = f_opendir(pDirObject, path);

	//
	// Check for error and return if there is a problem.
	//
	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	xSemaphoreGive(FSSema);
	return TRUE;
}

//*****************************************************************************
//! \param[in] pDirObject directory handle.
//! \param[out] pFileInfo pointer to an empty structure used to get the directory entry.
//!
//! Reads the directory entries in sequence. All items in the directory can be read by calling \e FSReadDir function repeatedly.
//! When all directory items have been read the function returns \b FALSE.
//!
//! \return Returns \b TRUE if ok or \b FALSE fails or no more items in the directory.
//*****************************************************************************
BOOL FSReadDir(DIR_OBJECT *pDirObject, FILE_INFO *pFileInfo)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	fresult = f_readdir(pDirObject, pFileInfo);

	//
	// Check for error and return if there is a problem.
	//
	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	//
	// If the file name is blank, then this is the end of the
	// listing.
	//
	if (!pFileInfo->fname[0])
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	xSemaphoreGive(FSSema);
	return TRUE;
}

//*****************************************************************************
//! \param[in] name zero terminated string containing the full directory path.
//!
//! This function creates a directory.
//!
//!
//! \return Returns \b TRUE if ok or \b FALSE fails.
//*****************************************************************************
BOOL FSCreateDir(char *name)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	fresult = f_mkdir(name);

	//
	// Check for error and return if there is a problem.
	//
	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	xSemaphoreGive(FSSema);
	return TRUE;
}

//*****************************************************************************
//! \param[in] name zero terminated string containing the full directory  or file path.
//!
//! This function deletes a file or a directory.
//!
//!
//! \return Returns \b TRUE if ok or \b FALSE fails.
//*****************************************************************************
BOOL FSDelete(char *name)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	fresult = f_unlink(name);

	//
	// Check for error and return if there is a problem.
	//
	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	xSemaphoreGive(FSSema);
	return TRUE;
}

//*****************************************************************************
//! Format the disk.
//!
//!
//! \return Returns \b TRUE if ok or \b FALSE fails.
//*****************************************************************************
BOOL FSFormat(void)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	fresult = f_mkfs(0, 0, 64);

	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	xSemaphoreGive(FSSema);
	return TRUE;
}

//*****************************************************************************
//! \param[in] path Root directory of the drive.
//! \param[in] clusters Pointer to the variable to store number of free clusters.
//! \param[in] fsObj Pointer to pointer to file system object.
//!
//!
//!
//! Get free space
//!
//!
//! \return Returns \b TRUE if ok or \b FALSE fails.
//*****************************************************************************
BOOL FSGetFree(char* path, LONG *clusters, FATFS** fsObj)
{
	FRESULT fresult;

	if (xSemaphoreTake(FSSema, TICK_MS(1000)) != pdTRUE)
	{
		return FALSE;
	}

	fresult = f_getfree(path, clusters, fsObj);

	if (fresult != FR_OK)
	{
		xSemaphoreGive(FSSema);
		return FALSE;
	}

	xSemaphoreGive(FSSema);
	return TRUE;
}

//*****************************************************************************
//! Close the Doxygen group.
//! @}
//*****************************************************************************
