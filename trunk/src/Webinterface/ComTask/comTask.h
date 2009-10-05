#ifndef COMTASK_H
#define COMTASK_H

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


/** Message for the ComTask queue */
typedef struct
{
	char *command; //e.g. 'get', 'set'
	char *item; // name of the selected item
} xCOMMessage;


/* The queue used to send messages to the Communication task. */
xQueueHandle xCOMQueue;

/* ComTask stack size */
#define COM_STACK_SIZE			( configMINIMAL_STACK_SIZE + 10 )

/* The maximum number of message that can be waiting  at any one time. */
#define COM_QUEUE_SIZE					( 3 )

void vComTask( void *pvParameters );

#endif /* COMTASK_H */
