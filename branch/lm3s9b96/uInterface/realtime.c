/*
 * realtime.c
 *
 *  Created on: 15.11.2009
 *      Author: root
 */
#include <stdio.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "realtime.h"

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
	systemtime = t_new;
}

/*
 * Getter for the RealTimeClock returning as String
 */
char* get_dateandtime(char * pcBuf, int iBufLen) {
	snprintf(pcBuf, iBufLen, ctime(&systemtime));
	printf("Hole Zeit: %s", pcBuf);
	return pcBuf;
}
