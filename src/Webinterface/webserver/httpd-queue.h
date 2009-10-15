

/** Message for the httpd queue */
typedef struct
{
	char *msg; // message back
} xHTTPDMessage;


/* The queue used to send messages to the Communication task. */
xQueueHandle xHTTPDQueue;

/* ComTask stack size */
#define HTTPD_STACK_SIZE			( configMINIMAL_STACK_SIZE * 3 )

/* The maximum number of message that can be waiting  at any one time. */
#define HTTPD_QUEUE_SIZE					( 3 )
