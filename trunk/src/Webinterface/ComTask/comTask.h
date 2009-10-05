/**
 * \addtogroup comTask
 * @{
 *
 * \file comTask.c
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

#include "lcd_message.h"  /* Include xOLEDMessage */


#define COM_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 10 )

/* The maximum number of message that can be waiting  at any one time. */
#define COM_TASK_QUEUE_SIZE					( 3 )

void vComTask( void *pvParameters );
