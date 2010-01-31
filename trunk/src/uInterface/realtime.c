/*
 * realtime.c
 *
 *  Created on: 15.11.2009
 *      Author: root
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "realtime.h"
#include "lwip/sntp.h"

#include "configuration/configloader.h"

time_t systemtime;
/*
 * Task for the RealTimeClock
 */
void vRealTimeClockTask(void * pvParameters) {
	portTickType xLastWakeTime;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	printf("realtime clock activated\n");
	systemtime = 0;
	for (;;) {
		// Wait for the next cycle.
		vTaskDelayUntil(&xLastWakeTime, 1000 / portTICK_RATE_MS);
		systemtime++;
	}
}

/*
 * Setter for the RealTimeClock
 */
void vSetRealTimeClock(time_t t_new) {
	vTaskSuspendAll();
	{
		int addSec = 0;
		char *configLoad = loadFromConfig(IP_CONFIG_FILE, "TIME_ZONE");
		if (configLoad != NULL) {
			addSec = atoi(configLoad) * 3600;

			vPortFree(configLoad);
		}
		systemtime = t_new + addSec;
	}
	xTaskResumeAll();
}

/*
 * Getter for the RealTimeClock returning as String
 */
char* get_dateandtime(char * pcBuf, int iBufLen) {
	snprintf(pcBuf, iBufLen, ctime(&systemtime));
	printf("vRealTimeClockTask: get time %s", pcBuf);
	return pcBuf;
}
