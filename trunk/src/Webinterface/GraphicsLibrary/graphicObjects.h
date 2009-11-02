/**
 * \addtogroup GraphicsLibrary
 * @{
 *
 * \file graphicObjects.h
 * \author Anziner, Hahn
 * \brief This Libary is used to draw the Graphic Objects to different Display Types.
 *
 */

#ifndef GRAPHICOBJECTS_H_
#define GRAPHICOBJECTS_H_

/* queue includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* HW includes */
#include "portmacro.h"

xTaskHandle xGraphicObjectsTaskHandler;

/** Message for the Graph Task queue */
typedef struct
{
	char *msg;
} xGraphMessage;

typedef struct {
	int value;
} xGraphCommunication;

typedef struct
{
	long key;
} xGraphCommandMessage;

/* The queue used to send messages to the Graphics task. */
xQueueHandle xGraphQueue;
xQueueHandle xGraphCommandQueue;
xQueueHandle xGraphCommunicationQueue;

/* Graphics Task stack size */
#define GRAPH_STACK_SIZE			( configMINIMAL_STACK_SIZE * 3 )

/* The maximum number of message that can be waiting  at any one time. */
#define GRAPH_QUEUE_SIZE					( 3 )

static const unsigned char g_pucRIT128x96x4HorizontalInc[] =
{ 0xA0, 0x52 };

#ifndef NULL
#define NULL (void *) 0
#endif

#define UP GPIO_PIN_0
#define DOWN GPIO_PIN_1
#define RIGHT GPIO_PIN_3
#define LEFT GPIO_PIN_2
#define SELECT GPIO_PIN_1

#define BUTTON_UP     	0
#define BUTTON_DOWN   	1
#define BUTTON_LEFT   	2
#define BUTTON_RIGHT  	3
#define BUTTON_SELECT 	4

#define RELOAD			5

#define CHAR_WIDTH	5
#define CHAR_HEIGHT 7

#define VISIBLE_TOP 	0
#define VISIBLE_LEFT 	0
#define VISIBLE_WIDTH	108
#define VISIBLE_HEIGHT	86

#define OBJECT_HEIGHT	(CHAR_HEIGHT + 2)

#define ROW_HEIGHT (OBJECT_HEIGHT + 4)

#define LINE_HEIGHT (ROW_HEIGHT + 2)

//
// Graphic Definitions
//

static const unsigned char pucBorderNormal[] =
{ 0x00, 0x00, 0x00, 0x00 };
static const unsigned char pucBorderSelected[] =
{ 0x06, 0x06, 0x0C, 0x0C };
static const unsigned char pucBorderClicked[] =
{ 0x0C, 0x0C, 0x06, 0x06 };
static const unsigned char pucBorderDeactivated[] =
{ 0x0F, 0x0F, 0x0F, 0x0F };

static const unsigned char goButtonUp[] =
{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
		0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0x0F, 0xFF,
		0xFF, 0xF0, 0xFF, 0x0F, 0xFF, 0xFF, 0x0F, 0xFF, 0xF0, 0xFF, 0xFF, 0x0F,
		0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, };

static const unsigned char goButtonDown[] =
{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF,
		0xF0, 0xFF, 0xFF, 0x0F, 0xFF, 0xF0, 0xFF, 0xFF, 0xF0, 0xFF, 0x0F, 0xFF,
		0xFF, 0xF0, 0xFF, 0x0F, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
		0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF };

#endif /* GRAPHICOBJECTS_H_ */
