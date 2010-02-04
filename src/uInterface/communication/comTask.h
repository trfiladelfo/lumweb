/*
 * comTask.h $Rev: 146 $ $Author: martin.anzinger $
 * $LastChangedDate: 2009-11-11 11:52:35 +0100 (Mi, 11 Nov 2009) $
 *
 * GPV v3
 *
*/

/**
 * \addtogroup comTask
 * @{
 *
 * \file comTask.h
 * \author Anziner, Hahn
 * \brief defines the Communication Task (vComTask) to interact with the machine
 *
*/

#ifndef COMTASK_H
#define COMTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "hw_types.h"


/** Command enummeration */
enum com_commands {SET, GET};

/** Datasource enummeraiton */
enum com_dataSource {CONF, DATA};


/** Message for the ComTask queue */
typedef struct
{
	enum com_commands cmd; ///e.g. 'get', 'set'
	enum com_dataSource dataSouce; /// e.g. 'conf', 'data'
	char *item; /// name of the selected item
	int value; /// value if a Item is set
	char *errorDesc; /// if not null, an error has occoured
	tBoolean freeItem; /// if true, free item in ComTask
	xQueueHandle from; /// address to return answer (name of the Queue)
	xTaskHandle taskToResume; /// If not null the specific task will be resumed
} xComMessage;

/** Prototype for the CommTask */
void vComTask( void *pvParameters );

#endif /* COMTASK_H */
