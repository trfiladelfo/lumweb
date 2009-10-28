#ifndef COMTASK_H
#define COMTASK_H

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

enum com_from {HTTPD, GRAPHIC};
/** Message for the ComTask queue */
typedef struct
{
	enum com_commands cmd; //e.g. 'get', 'set'
	char *item; // name of the selected item
	int value; // value if a Item ist set
	enum com_from from; // adress to return answer
} xCOMMessage;


/* The queue used to send messages to the Communication task. */
xQueueHandle xCOMQueue;

/* ComTask stack size */
#define COM_STACK_SIZE			( configMINIMAL_STACK_SIZE * 3 )

/* The maximum number of message that can be waiting  at any one time. */
#define COM_QUEUE_SIZE					( 3 )

void vComTask( void *pvParameters );

#endif /* COMTASK_H */
