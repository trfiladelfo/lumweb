/*
 * comTask.h $Rev$ $Author$ $LastChangedDate$
 *
 * (c) 2009 Anziner, Hahn
 * GPV v3
 *
 * defines the Communication Task (vComTask) to interact with the machine
*/

#include "lcd_message.h"  /* Include xOLEDMessage */


#define Com_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 10 )


void vComTask( void *pvParameters );
