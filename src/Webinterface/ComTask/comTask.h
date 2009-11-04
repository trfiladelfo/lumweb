#ifndef COMTASK_H
#define COMTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/**
 * \addtogroup comTask
 * @{
 *
 * \file comTask.h
 * \author Anziner, Hahn
 * \brief defines the Communication Task (vComTask) to interact with the machine
 *
*/

/*
 * comTask.h $Rev$ $Author$
 * $LastChangedDate$
 *
 * GPV v3
 *
*/

/* Command enummeration */
enum com_commands {SET, GET};

/* Datasource enummeraiton */
enum com_dataSource {CONF, DATA};

/* Response address */
//enum com_from {HTTPD, GRAPHIC}; // REPLACED THROUGH ADRESS OF THE RESPONSE QUEUE

/** Message for the ComTask queue */
typedef struct
{
	enum com_commands cmd; //e.g. 'get', 'set'
	enum com_dataSource dataSouce; // e.g. 'conf', 'data'
	char *item; // name of the selected item
	int value; // value if a Item is set
	char *errorDesc; // if not null, an error has occoured
	xQueueHandle from; // address to return answer (name of the Queue)
	xTaskHandle taskToResume; // If not null the specific task will be resumed
} xCOMMessage;


/* The queue used to send messages to the Communication task. */
xQueueHandle xCOMQueue;

/* ComTask stack size */
#define COM_STACK_SIZE			( configMINIMAL_STACK_SIZE * 3 )

/* The maximum number of message that can be waiting  at any one time. */
#define COM_QUEUE_SIZE					( 6 )

/* Prototype for the CommTask */
void vComTask( void *pvParameters );

#endif /* COMTASK_H */
