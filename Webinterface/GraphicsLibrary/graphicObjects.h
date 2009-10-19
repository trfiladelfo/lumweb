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

/** Message for the Graph Task queue */
typedef struct {
	char *msg; //e.g. 'get', 'set'
} xGraphMessage;

typedef struct {
	long key;
} xGraphCommandMessage;

/* The queue used to send messages to the Graphics task. */
xQueueHandle xGraphQueue;
xQueueHandle xGraphCommandQueue;

/* Graphics Task stack size */
#define GRAPH_STACK_SIZE			( configMINIMAL_STACK_SIZE * 3 )

/* The maximum number of message that can be waiting  at any one time. */
#define GRAPH_QUEUE_SIZE					( 3 )

static const unsigned char g_pucRIT128x96x4HorizontalInc[] = { 0xA0, 0x52 };

#ifndef NULL
#define NULL (void *) 0
#endif

#define UP GPIO_PIN_0
#define DOWN GPIO_PIN_1
#define RIGHT GPIO_PIN_3
#define LEFT GPIO_PIN_2
#define SELECT GPIO_PIN_1

#define BUTTON_UP     0
#define BUTTON_DOWN   1
#define BUTTON_LEFT   2
#define BUTTON_RIGHT  3
#define BUTTON_SELECT 4

struct goButton {
	int height;
	int width;
	int left;
	int top;
	const char unsigned* value;
	struct goButton * next;
	void (*selectAction)(void);
};

typedef struct goButton * pgoButton;

/*
 * A Query to
 */
pgoButton buttonListRoot;
pgoButton buttonListLast;
pgoButton buttonSelected;

static const unsigned char pucBorderNormal[] = { 0x06, 0x0C, 0x06, 0x0C };
static const unsigned char pucBorderSelected[] = { 0x06, 0x06, 0x0C, 0x0C };
static const unsigned char pucBorderClicked[] = { 0x0C, 0x0C, 0x06, 0x06 };
static const unsigned char pucBorderDeactivated[] = { 0x0F, 0x0F, 0x0F, 0x0F };

void goInit(void); // Initializes the GraphicLibary

pgoButton goNewButton(int height, int width, int left, int top,
		const char unsigned* value, void(*selectAction)(void)); // Creates a new Button

pgoButton goDrawNewButton(int height, int width, int left, int top,
		const char unsigned* value); // Creates a new Button

void goDrawButton(pgoButton btn, unsigned const char* type); // Draws the Button

void goDrawButtons(void); // Draws all Buttons

void goDeleteButton(pgoButton btn); // Deletes the Button

void goStartListener(xTaskHandle handler); // Starts the Listener

void goDrawBorder(int height_, int width_, int left, int top,
		unsigned const char * type); // Draws a Border for a Object

pgoButton goGetNextButton(pgoButton btn);
pgoButton goGetLastButton(void);
pgoButton goGetPrevButton(pgoButton btn);
pgoButton goGetFirstButton(void);

void goInsertButton(pgoButton btn);
void goRemoveButton(pgoButton btn);

static const unsigned char goButtonUp[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
		0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0x0F, 0xFF, 0xFF, 0xF0, 0xFF, 0x0F, 0xFF,
		0xFF, 0x0F, 0xFF, 0xF0, 0xFF, 0xFF, 0x0F, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, };

static const unsigned char goButtonDown[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0xF0, 0xFF, 0xFF, 0x0F,
		0xFF, 0xF0, 0xFF, 0xFF, 0xF0, 0xFF, 0x0F, 0xFF, 0xFF, 0xF0, 0xFF, 0x0F,
		0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

#endif /* GRAPHICOBJECTS_H_ */