#ifndef __SNTP_H__
#define __SNTP_H__

#include <string.h>

#define SNTP_SYSTEM_TIME(t) \
extern void vSetRealTimeClock ( time_t t_new ); \
vSetRealTimeClock ( t );

//  static xOLEDMessage xOLEDMessage; \
//  static portCHAR cMessageForDisplay[MAX_OLED_MSG_LEN]; \
//  extern xQueueHandle xOLEDQueue; \
// \
//  strftime( cMessageForDisplay, 21, "%x %X", localtime(&t)); \
//	xOLEDMessage.pcMessage = (signed portCHAR *)cMessageForDisplay; \
//  xQueueSend( xOLEDQueue, &xOLEDMessage, portMAX_DELAY );


void sntp_init(void);

#endif /* __SNTP_H__ */
