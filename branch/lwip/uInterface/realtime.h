/*
 * realtime.h
 *
 *  Created on: 15.11.2009
 *      Author: root
 */

#ifndef REALTIME_H_
#define REALTIME_H_

#include <time.h>

extern time_t systemtime; // Global System Time

void vRealTimeClockTask(void * pvParameters);

void vSetRealTimeClock(time_t t_new);

char* get_dateandtime(char * pcBuf, int iBufLen);

#endif /* REALTIME_H_ */
