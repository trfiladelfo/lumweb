/**
 * \addtogroup System
 * @{
 *
 * \author Anziner, Hahn
 * \brief Functions for the System Clock
 *
 *
 */

#ifndef REALTIME_H_
#define REALTIME_H_

#include <time.h>

/**
 * global space for the system time
 */
extern time_t systemtime;

/**
 * Task function for the system clock
 *
 * @param pvParameters Pointer for userdefined parameters of the task
 */
void vRealTimeClockTask(void * pvParameters);


/**
 * setter for the Time (callback for the LWIP NTP function)
 *
 * @param t_new new Time which has to be set
 */
void vSetRealTimeClock(time_t t_new);

/**
 * Returns the actual time as String
 *
 * @param pcBuf C-String for saving the string
 * @param iBufLen Length of the string buffer
 *
 * @return pcBuf
 */
char* get_dateandtime(char * pcBuf, int iBufLen);

#endif /* REALTIME_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

