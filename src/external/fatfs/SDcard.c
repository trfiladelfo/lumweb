/*-----------------------------------------------------------------------*/
/* MMC/SDC (in SPI mode) control module  (C)PF, 2009                     */
/*-----------------------------------------------------------------------*/

/*
 * This file was modified from a sample available from the FatFs
 * web site. It was modified to work with a Luminary Micro
 * EK-LM3S8962 evaluation board.
 *
 * Note that the SSI port is shared with the OLED display. The code
 * in this file does not attempt to share the SSI port with the display,
 * it assumes the display is not being used.
 */

#include <stdio.h>
#include <string.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "ff.h"

#include "hw_memmap.h"
#include "hw_types.h"
#include "sysctl.h"
#include "gpio.h"

// SSI port
#define SDC_SSI_BASE            SSI0_BASE
#define SDC_SSI_SYSCTL_PERIPH   SYSCTL_PERIPH_SSI0

// GPIO for SSI pins
#define SDC_GPIO_PORT_BASE      GPIO_PORTA_BASE
#define SDC_GPIO_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOA
#define SDC_SSI_CLK             GPIO_PIN_2
#define SDC_SSI_TX              GPIO_PIN_5
#define SDC_SSI_RX              GPIO_PIN_4
#define SDC_SSI_FSS             GPIO_PIN_3
#define SDC_SSI_PINS            (SDC_SSI_TX | SDC_SSI_RX | SDC_SSI_CLK)

// GPIO for card chip select
#define SDC_CS_GPIO_PORT_BASE      GPIO_PORTG_BASE
#define SDC_CS_GPIO_SYSCTL_PERIPH  SYSCTL_PERIPH_GPIOG
#define SDC_CS                     GPIO_PIN_0

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
// This buffer holds the full path to the current working directory.
// Initially it is root ("/").
//
//*****************************************************************************
//ma static char g_cCwdBuf[PATH_BUF_SIZE] = "/";

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;
//ma static DIR g_sDirObject;
//ma static FILINFO g_sFileInfo;

void disk_timerproc(void);
/*-----------------------------------------------------------------------*/
/* init the micro SD card driver and                                     */
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This timer function must be called in period of 10ms                  */
/*-----------------------------------------------------------------------*/
void vSDcardTask(void *pvParameters) {
	//
	// Enable the peripherals used by the micro SD card.
	//
	SysCtlPeripheralEnable(SDC_SSI_SYSCTL_PERIPH);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//
	// Mount the file system, using logical disk 0.
	//
	if (f_mount(0, &g_sFatFs) != FR_OK) {
		printf("f_mount error\n");
		vTaskDelete(NULL);
	}
	for (;;) {
		disk_timerproc();
		vTaskDelay(10 / portTICK_RATE_MS);
	}
}
