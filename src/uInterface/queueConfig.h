/**
 * \addtogroup Configuration
 * @{
 *
 * \author Anziner, Hahn
 * \brief Configfile for queues
 *
 *
 */

#ifndef QUEUECONFIG_H_
#define QUEUECONFIG_H_

#include "FreeRTOS.h"
#include "queue.h"

//*****************************************************************************
//
// COM Queue
//
//*****************************************************************************
/// Size of the COM Queue
#define COM_QUEUE_SIZE 		6

/// Queuehandler for the COM Queue
xQueueHandle xComQueue;

//*****************************************************************************
//
// HTTDP Queue
//
//*****************************************************************************
/// Size of the HTTPD Queue
#define HTTPD_QUEUE_SIZE 	6

/// Queuehandler for the HTTPD Queue
xQueueHandle xHttpdQueue;


#endif /* QUEUECONFIG_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

