/*
 * httpTask.h
 *
 *  Created on: Nov 4, 2009
 *      Author: d3f3nd3r
 */

#ifndef HTTPTASK_H_
#define HTTPTASK_H_

#include "FreeRTOS.h"
#include "task.h"

/* Size of the stack allocated to the uIP task. */
#define HTTPD_STACK_SIZE           ( configMINIMAL_STACK_SIZE * 5 )

#define HTTPD_QUEUE_SIZE					( 3 )

xQueueHandle xHTTPDQueue;

void vHttpdTask(void *pvParameters);

xTaskHandle xHttpTaskHandler;


#endif /* HTTPTASK_H_ */
