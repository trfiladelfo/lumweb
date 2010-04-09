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

/// Task handle for LWIP, used for suspending and resuming tasks
xTaskHandle xLwipTaskHandle;

/// Stack size for the Graphic Task (Graphic user Interface)
#define GRAPH_STACK_SIZE	512

/// Taskname for the Graphic Task
#define GRAPH_TASK_NAME		"graphics"

/// Task priority for the Graphic Task
#define GRAPH_TASK_PRIORITY  (configMAX_PRIORITIES - 2)

/// Task handle for Graphic Task, used for suspending and resuming tasks
xTaskHandle xGraphTaskHandle;

/// Stack size for the Communication Task
#define COM_STACK_SIZE		128 * 2

/// Task name for the Communication Task
#define COM_TASK_NAME		"com"

/// Task priority for the Communication Task
#define COM_TASK_PRIORITY  (configMAX_PRIORITIES - 2)

/// Task handler for the Communication Task
xTaskHandle xComTaskHandle;


/// Stack size for the SD-Card Manager Task
#define SDCARD_STACK_SIZE		128

/// Task name for the SD-Card Manager Task
#define SDCARD_TASK_NAME		"sdcard"

/// Task priority (0 = idle)
#define SDCARD_TASK_PRIORITY  	0

/// Startpoint for the SD-Card Manager Task
extern void vSDcardTask(void *pvParameters);

/// Task handler for the SD-Card Manager Task
xTaskHandle xSdCardTaskHandle;


/// Stack size for the Clock Task
#define TIME_STACK_SIZE		128

/// Task name for the Clock Task
#define TIME_TASK_NAME		"clock"

/// Task priority (configMAX_PRIORITIES equals the maximum priority)
#define TIME_TASK_PRIORITY  configMAX_PRIORITIES

/// Task handler for the Clock Task
xTaskHandle xRealtimeTaskHandle;

#endif /* TASKCONFIG_H_ */
