/**
 * \addtogroup Configuration
 * @{
 *
 * \file queueConfig.h
 * \author Anziner, Hahn
 * \brief Configfile for tasks
 *
 *
 */

#ifndef TASKCONFIG_H_
#define TASKCONFIG_H_

#include "FreeRTOSConfig.h"

/// Stack size for the LWIP Task (also the Webserver)
#define LWIP_STACK_SIZE		512*2
/// Task name for the LWIP Task
#define LWIP_TASK_NAME		"lwip"
/// Priority for LWIP Task
#define LWIP_TASK_PRIORITY  (configMAX_PRIORITIES - 2)
#define LWIP_USE_DHCP		0
/// Task handle for LWIP, used for suspending and resuming tasks
xTaskHandle xLwipTaskHandle;

#define GRAPH_STACK_SIZE	512
#define GRAPH_TASK_NAME		"graphics"
#define GRAPH_TASK_PRIORITY  (configMAX_PRIORITIES - 2)
xTaskHandle xGraphTaskHandle;

#define COM_STACK_SIZE		128 * 2
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
