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
static char g_cCwdBuf[PATH_BUF_SIZE] = "/";

//*****************************************************************************
//
// The following are data structures used by FatFs.
//
//*****************************************************************************
static FATFS g_sFatFs;
static DIR g_sDirObject;
static FILINFO g_sFileInfo;

void disk_timerproc (void);
int Cmd_ls(char * buffer, int BufferSize, char * NewPath);
/*-----------------------------------------------------------------------*/
/* init the micro SD card driver and                                     */
/* Device Timer Interrupt Procedure  (Platform dependent)                */
/*-----------------------------------------------------------------------*/
/* This timer function must be called in period of 10ms                  */
/*-----------------------------------------------------------------------*/
void vSDcardTask( void *pvParameters )
{
    //
    // Enable the peripherals used by the micro SD card.
    //
    SysCtlPeripheralEnable(SDC_SSI_SYSCTL_PERIPH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //
    // Mount the file system, using logical disk 0.
    //
    if(f_mount(0, &g_sFatFs) != FR_OK)
    {
        printf("f_mount error\n");
        vTaskDelete( NULL );
    }
	  for( ;; )
	  {
		    disk_timerproc();
        vTaskDelay( 10/portTICK_RATE_MS );
		}
}

//*****************************************************************************
//
// This function implements the "ls" command.  It opens the current
// directory and enumerates through the contents, and prints a line for
// each item it finds.  It shows details such as file attributes, time and
// date, and the file size, along with the name.
//
//*****************************************************************************
int
Cmd_ls(char * buffer, int BufferSize, char * NewPath)
{
  	int CharacterCount = 0;
		char * linkPath;
    char CwdBuf[PATH_BUF_SIZE];
    FRESULT fresult;

		//
		// extract path from url
		//
		if (NewPath[strlen(NewPath) - 1] == '/')
		{
			strncpy(g_cCwdBuf, strchr(NewPath + 1, '/'), PATH_BUF_SIZE);
			if(strlen(g_cCwdBuf) > 1)
			{
		    g_cCwdBuf[strlen(g_cCwdBuf) - 1] = '\0';
			}
		}

    //
    // Open the current directory for access.
    //
    fresult = f_opendir(&g_sDirObject, g_cCwdBuf);

    //
    // Check for error and return if there is a problem.
    //
    if(fresult != FR_OK)
    {
	    CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
			                            "\n<tr><td>not able to mount directory</td></tr>");
		  if (CharacterCount >= BufferSize)
			  return (BufferSize);
	    return (CharacterCount);
    }

    CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
		                            "\n<tr><td>Path</td><td colspan=\"8\"><a href=\"/Logging.html/\">/ </a>");
	  if (CharacterCount >= BufferSize)
		  return (BufferSize);
		linkPath = g_cCwdBuf;
		for (;;)
		{
		  if (strlen(linkPath) == 1) //root Directory
			  break;
			if (strchr(linkPath + 1, '/') == NULL) // current Directory is the last one
			{
	      CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
			                              "%s",
																		linkPath + 1);
			  if (CharacterCount >= BufferSize)
				  return (BufferSize);
			  break;
			}
			memset(CwdBuf, '\0', PATH_BUF_SIZE);
		  strncpy (CwdBuf, g_cCwdBuf, strchr(linkPath + 1, '/') - g_cCwdBuf);
      CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
		                              "<a href=\"/Logging.html%s/\">",
																	CwdBuf);
		  if (CharacterCount >= BufferSize)
			  return (BufferSize);
			memset(CwdBuf, '\0', PATH_BUF_SIZE);
	    strncpy (CwdBuf, linkPath + 1, strchr(linkPath + 1, '/') - linkPath);
			linkPath = strchr(linkPath + 1, '/');
      CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
		                              "%s </a>",
																	CwdBuf);
		  if (CharacterCount >= BufferSize)
			  return (BufferSize);

		}
    CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
		                            "</td></tr>");
	  if (CharacterCount >= BufferSize)
		  return (BufferSize);
    CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
		                            "\n<tr><td>Name</td><td colspan=\"5\">Attributes</td><td>Date</td><td>Time</td><td>Size</td></tr>");
	  if (CharacterCount >= BufferSize)
		  return (BufferSize);
    //
    // Enter loop to enumerate through all directory entries.
    //
    for(;;)
    {
        //
        // Read an entry from the directory.
        //
        fresult = f_readdir(&g_sDirObject, &g_sFileInfo);

        //
        // Check for error and return if there is a problem.
        //
        if(fresult != FR_OK)
        {
			    CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
					                            "\n<tr><td>f_readdir failed</td></tr>");
				  if (CharacterCount >= BufferSize)
					  return (BufferSize);
			    return (CharacterCount);
        }

        //
        // If the file name is blank, then this is the end of the
        // listing.
        //
        if(!g_sFileInfo.fname[0])
        {
            break;
        }

				if (strlen(g_cCwdBuf) > 1)
				{
					g_cCwdBuf[strlen(g_cCwdBuf) + 1] = '\0';
					g_cCwdBuf[strlen(g_cCwdBuf)] = '/';
				}
        //
        // Print the entry information on a single line with formatting
        // to show the attributes, date, time, size, and name.
        //
        CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
				                            "\n<tr><td><a href=\"/Logging.html%s%s%c\">%s</a></td><td>%c</td><td>%c</td><td>%c</td><td>%c</td><td>%c</td><td>%u.%02u.%02u</td><td>%02u:%02u</td><td>%u</td></tr>",
																		 g_cCwdBuf,
								                     g_sFileInfo.fname,
																		(g_sFileInfo.fattrib & AM_DIR) ? '/' : ' ',
								                     g_sFileInfo.fname,
								                    (g_sFileInfo.fattrib & AM_DIR) ? 'D' : '-',
								                    (g_sFileInfo.fattrib & AM_RDO) ? 'R' : '-',
								                    (g_sFileInfo.fattrib & AM_HID) ? 'H' : '-',
								                    (g_sFileInfo.fattrib & AM_SYS) ? 'S' : '-',
								                    (g_sFileInfo.fattrib & AM_ARC) ? 'A' : '-',
								                     g_sFileInfo.fdate & 31,
								                    (g_sFileInfo.fdate >> 5) & 15,
								                    (g_sFileInfo.fdate >> 9) + 1980,
								                    (g_sFileInfo.ftime >> 11),
								                    (g_sFileInfo.ftime >> 5) & 63,
								                     g_sFileInfo.fsize);
			  if (CharacterCount >= BufferSize)
				  return (BufferSize);
				if (strlen(g_cCwdBuf) > 2)
				{
					g_cCwdBuf[strlen(g_cCwdBuf) - 1] = '\0';
				}
    }   // endfor

    //
    // Check for error and return if there is a problem.
    //
    if(fresult != FR_OK)
    {
	    CharacterCount += _snprintf(buffer + CharacterCount, BufferSize - CharacterCount,
			                            "\n<tr><td>Directory listing failed</td></tr>");
		  if (CharacterCount >= BufferSize)
			  return (BufferSize);
	    return (CharacterCount);
    }

    //
    // Made it to here, return with no errors.
    //
    return(CharacterCount);
}
