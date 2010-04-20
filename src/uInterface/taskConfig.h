/**
 * \addtogroup Configuration
 * @{
 *
 * \author Anziner, Hahn
 * \brief Configfile for tasks
 *
 *
 */

#ifndef TASKCONFIG_H_
#define TASKCONFIG_H_

#include "FreeRTOSConfig.h"

//*****************************************************************************
//
// LWIP Task
//
//*****************************************************************************

/// Stack size for the LWIP Task (the Webserver)
#define LWIP_STACK_SIZE		512*2

/// Task name for the LWIP Task
#define LWIP_TASK_NAME		"lwip"

/// Priority for LWIP Task
#define LWIP_TASK_PRIORITY  (configMAX_PRIORITIES - 2)

/// Task handle for LWIP, used for suspending and resuming tasks
xTaskHandle xLwipTaskHandle;


//*****************************************************************************
//
// Graphic Task
//
//*****************************************************************************

/// Stack size for the Graphic Task (Graphic user Interface)
#define GRAPH_STACK_SIZE	512

/// Taskname for the Graphic Task
#define GRAPH_TASK_NAME		"graphics"

/// Task priority for the Graphic Task
#define GRAPH_TASK_PRIORITY  (configMAX_PRIORITIES - 2)

/// Task handle for Graphic Task, used for suspending and resuming tasks
xTaskHandle xGraphTaskHandle;


//*****************************************************************************
//
// Com Task
//
//*****************************************************************************

/// Stack size for the Communication Task
#define COM_STACK_SIZE		128 * 2

/// Task name for the Communication Task
#define COM_TASK_NAME		"com"

/// Task priority for the Communication Task
#define COM_TASK_PRIORITY  (configMAX_PRIORITIES - 2)

/// Task handler for the Communication Task
xTaskHandle xComTaskHandle;


//*****************************************************************************
//
// Real Time Task
//
//*****************************************************************************
/// Stack size for the Clock Task
#define TIME_STACK_SIZE		128

/// Task name for the Clock Task
#define TIME_TASK_NAME		"clock"

/// Task priority (configMAX_PRIORITIES equals the maximum priority)
#define TIME_TASK_PRIORITY  configMAX_PRIORITIES

/// Task handler for the Clock Task
xTaskHandle xRealtimeTaskHandle;

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


#endif /* TASKCONFIG_H_ */
