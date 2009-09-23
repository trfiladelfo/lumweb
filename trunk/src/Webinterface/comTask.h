/*
defines the Communication Task (vComTask) to interact with the machine
*/

#define Com_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE + 10 )

void vComTask( void *pvParameters );
