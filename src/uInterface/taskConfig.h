/*
 * taskConfig.h
 *
 *  Created on: 14.11.2009
 *      Author: root
 */

#ifndef TASKCONFIG_H_
#define TASKCONFIG_H_

#include "FreeRTOSConfig.h"

#define LWIP_STACK_SIZE		512
#define LWIP_TASK_NAME		"lwip"
#define LWIP_TASK_PRIORITY  (configMAX_PRIORITIES - 2)
#define LWIP_USE_DHCP		0
xTaskHandle xLwipTaskHandle;

#define GRAPH_STACK_SIZE	512
#define GRAPH_TASK_NAME		"graphics"
#define GRAPH_TASK_PRIORITY  (configMAX_PRIORITIES - 2)
xTaskHandle xGraphTaskHandle;

#define COM_STACK_SIZE		128
#define COM_TASK_NAME		"com"
#define COM_TASK_PRIORITY  (configMAX_PRIORITIES - 2)
xTaskHandle xComTaskHandle;

#define SDCARD_STACK_SIZE		128
#define SDCARD_TASK_NAME		"sdcard"
#define SDCARD_TASK_PRIORITY  	0
extern void vSDcardTask( void *pvParameters );
xTaskHandle xSdCardTaskHandle;

#define TIME_STACK_SIZE		128
#define TIME_TASK_NAME		"clock"
#define TIME_TASK_PRIORITY  configMAX_PRIORITIES
xTaskHandle xRealtimeTaskHandle;

#endif /* TASKCONFIG_H_ */
