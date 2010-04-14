/**
 * \addtogroup System
 * @{
 *
 * \file realtime.c
 * \author Anziner, Hahn
 * \brief Functions for the System Clock
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "realtime.h"
#include "lwip/sntp.h"

#include "configuration/configloader.h"

/**
 * Memory for the systemtime
 */
time_t systemtime;

/**
 * Task function for the system clock
 *
 * @param pvParameters Pointer for userdefined parameters of the task
 */
void vRealTimeClockTask(void * pvParameters)
{
	portTickType xLastWakeTime;

	//
	// Initialise the xLastWakeTime variable with the current time.
	//
	xLastWakeTime = xTaskGetTickCount();

	printf("realtime clock activated\n");

	//
	// initalize systemtime with 0
	//
	systemtime = 0;

	//
	// Endless loop with 1 second delay
	//
	for (;;)
	{
		//
		// Wait for the next cycle.
		//
		vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);

		//
		// increase the systemtime
		//
		systemtime++;
	}
}

/**
 * setter for the Time (callback for the LWIP NTP function)
 *
 * @param t_new new Time which has to be set
 */
void vSetRealTimeClock(time_t t_new)
{
	//
	// Suspend all Task (synchronize Tasks)
	//
	vTaskSuspendAll();
	{
		int addSec = 0;

		//
		// Open Config file and fetch the Timezone value
		//
		char *configLoad = loadFromConfig(IP_CONFIG_FILE, "TIME_ZONE");

		//
		// Check if Value is available
		//
		if (configLoad != NULL)
		{
			//
			// transform the config value to seconds
			//
			addSec = atoi(configLoad) * 3600;

			//
			// free the configuration file
			//
			vPortFree(configLoad);
		}

		//
		// set the new systemtime
		//
		systemtime = t_new + addSec;
	}
	xTaskResumeAll();
	//
	// resume all Tasks
	//
}

/**
 * Returns the actual time as String
 *
 * @param pcBuf C-String for saving the string
 * @param iBufLen Length of the string buffer
 *
 * @return pcBuf
 */
char* get_dateandtime(char * pcBuf, int iBufLen)
{
	//
	// convert time to string with the stdlib function ctime
	//
	snprintf(pcBuf, iBufLen, ctime(&systemtime));

	//
	// return string
	//
	return pcBuf;
}


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
