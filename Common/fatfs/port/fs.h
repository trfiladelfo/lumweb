#ifndef __FS_H__
#define __FS_H__

#include "ff.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void* 			FILE_HANDLE;
typedef FILINFO	 		FILE_INFO;
typedef DIR				DIR_OBJECT;
#define INVALID_FILE_HANDLE 0



//*****************************************************************************
// API
//! \defgroup FS_API FS API
//! @{
//! \file fs.h
//! \file fs.c
//*****************************************************************************
void FSInit(void);
FILE_HANDLE FSOpen(char *name);
FILE_HANDLE FSCreate(char *name);
BOOL FSClose(FILE_HANDLE handle);
int FSRead(FILE_HANDLE handle, void *buffer, int count);
int FSWrite(FILE_HANDLE handle, void *buffer, int count);
BOOL FSMoveReadPointer(FILE_HANDLE handle, int offset);
BOOL FSOpenDir(DIR_OBJECT *pDirObject, char *path);
BOOL FSReadDir(DIR_OBJECT *pDirObject, FILE_INFO *pFileInfo);
BOOL FSCreateDir(char *name);
BOOL FSDelete(char *name);
BOOL FSFormat(void);
BOOL FSGetFree(char* path, LONG* clusters, FATFS** fileSystemObject);
//*****************************************************************************
//! Close the Doxygen group.
//! @}
//*****************************************************************************

#ifdef __cplusplus
}
#endif


#endif /* __FS_H__ */

