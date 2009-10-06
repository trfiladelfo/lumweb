/*
 * filesystem.h
 *
 *  Created on: 06.10.2009
 *      Author: Anzinger
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "fs.h"

// System
#define SDC_SSI_BASE            SSI0_BASE
#define SDC_SSI_SYSCTL_PERIPH   SYSCTL_PERIPH_SSI0
// GPIO for SSI pins
#define SDC_GPIO_PORT_BASE      	GPIO_PORTA_BASE
#define SDC_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOA
#define SDC_SSI_CLK             	GPIO_PIN_2
#define SDC_SSI_TX              	GPIO_PIN_5
#define SDC_SSI_RX              	GPIO_PIN_4
#define SDC_SSI_FSS             	GPIO_PIN_3
#define SDC_SSI_PINS            	(SDC_SSI_TX | SDC_SSI_RX | SDC_SSI_CLK)

// GPIO for card chip select
#define SDC_CS_GPIO_PORT_BASE      	GPIO_PORTD_BASE
#define SDC_CS_GPIO_SYSCTL_PERIPH  	SYSCTL_PERIPH_GPIOD
#define SDC_CS                     	GPIO_PIN_0

#define SDC_MAX_SPEED				25000000

//*****************************************************************************
//
// Defines the size of the buffers that hold the path, or temporary
// data from the SD card.  There are two buffers allocated of this size.
// The buffer size must be large enough to hold the longest expected
// full path name, including the file name, and a trailing null character.
//
//*****************************************************************************
#define PATH_BUF_SIZE   80

//*****************************************************************************
//
// Defines the size of the buffer that holds the command line.
//
//*****************************************************************************
#define CMD_BUF_SIZE    64

//*****************************************************************************
//
// This buffer holds the full path to the current working directory.
// Initially it is root ("/").
//
//*****************************************************************************
static char g_cCwdBuf[PATH_BUF_SIZE] = "/";

//*****************************************************************************
//
// A temporary data buffer used when manipulating file paths, or reading data
// from the SD card.
//
//*****************************************************************************
static char g_cTmpBuf[PATH_BUF_SIZE];

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;
static DIR g_sDirObject;
static FILINFO g_sFileInfo;
static FIL g_sFileObject;

//*****************************************************************************
//
// A structure that holds a mapping between an FRESULT numerical code,
// and a string represenation.  FRESULT codes are returned from the FatFs
// FAT file system driver.
//
//*****************************************************************************
typedef struct {
	FRESULT fresult;
	char *pcResultStr;
} tFresultString;

//*****************************************************************************
//
// A macro to make it easy to add result codes to the table.
//
//*****************************************************************************
#define FRESULT_ENTRY(f)        { (f), (#f) }

//*****************************************************************************
//
// A table that holds a mapping between the numerical FRESULT code and
// it's name as a string.  This is used for looking up error codes for
// printing to the console.
//
//*****************************************************************************
tFresultString g_sFresultStrings[] = { FRESULT_ENTRY(FR_OK),
		FRESULT_ENTRY(FR_NOT_READY), FRESULT_ENTRY(FR_NO_FILE),
		FRESULT_ENTRY(FR_NO_PATH), FRESULT_ENTRY(FR_INVALID_NAME),
		FRESULT_ENTRY(FR_INVALID_DRIVE), FRESULT_ENTRY(FR_DENIED),
		FRESULT_ENTRY(FR_EXIST), FRESULT_ENTRY(FR_RW_ERROR),
		FRESULT_ENTRY(FR_WRITE_PROTECTED), FRESULT_ENTRY(FR_NOT_ENABLED),
		FRESULT_ENTRY(FR_NO_FILESYSTEM), FRESULT_ENTRY(FR_INVALID_OBJECT),
		FRESULT_ENTRY(FR_MKFS_ABORTED) };

//*****************************************************************************
//
// A macro that holds the number of result codes.
//
//*****************************************************************************
#define NUM_FRESULT_CODES (sizeof(g_sFresultStrings) / sizeof(tFresultString))

void vInitFileSystem(void);

#endif /* FILESYSTEM_H_ */
