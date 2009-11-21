/**
 * \addtogroup GraphicsLibrary
 * @{
 *
 * \file graphicObjects.c
 * \author Anziner, Hahn
 * \brief This Libary is used to draw the Graphic Objects to different Display Types.
 *
 */

/*
 *
 * This Libary is used to draw the Graphic Objects to different Display Types.
 *
 * Primary it is made to make selections with the menu buttons (UP, DOWN, LEFT, RIGHT, SELECT)
 *
 * It should also be able to make a touch Interface.
 */

//#include <string.h>

#include "graphicObjects.h"
#include "graphicTextBox.h"
#include "graphicButton.h"

#include "../taskConfig.h"
#include "../queueConfig.h"

#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_ints.h"

#include "gpio.h"
#include "sysctl.h"
#include "interrupt.h"

#include "rit128x96x4.h"

#include "startScreenImage.h"

portTickType xTicksLast = 0;

int top = 0;

void goDrawEmptyButton(int height, int width, int left, int top,
		unsigned const char * type);

/*
 * Statusvariables
 */
char isAlreadyInitilized = 0;

/**
 * Initializes the Display to Draw the Buttons
 */
void goInit() {
	if (!isAlreadyInitilized) {
		printf("Initialisiere Graphic...\n");
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		// Configure UP | DOWN | LEFT | RIGHT
		GPIOPadConfigSet(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT),
				GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT),
				GPIO_DIR_MODE_IN);

		// Configure SELECT
		GPIOPadConfigSet(GPIO_PORTF_BASE, SELECT, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTF_BASE, SELECT, GPIO_DIR_MODE_IN);

		GPIOIntTypeSet(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT),
				GPIO_FALLING_EDGE);
		GPIOPinIntEnable(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT));

		GPIOIntTypeSet(GPIO_PORTE_BASE, SELECT, GPIO_FALLING_EDGE);
		GPIOPinIntEnable(GPIO_PORTF_BASE, SELECT);

		xGraphCommandQueue = xQueueCreate(GRAPH_COMMAND_QUEUE_SIZE,
				sizeof(xGraphCommandMessage));

		RIT128x96x4Init(1000000);

		IntEnable(INT_GPIOE);
		IntEnable(INT_GPIOF);

		isAlreadyInitilized = 1;
	}
}

/*
 *
 */
void goObjectsListener(void) {
	xGraphCommandMessage xCommMessage;
	xGraphMessage xMessage;

	portTickType delay;

	printf("Starte Graphic Listener...\n");

	RIT128x96x4Clear();
	RIT128x96x4ImageDraw(g_pucStartScreenImage, (DISPLAY_WIDTH
			- g_uiStartScreenImageWidth) / 2, (DISPLAY_HEIGHT
			- g_uiStartScreenImageHeight) / 2, g_uiStartScreenImageWidth,
			g_uiStartScreenImageHeight);

	delay = xTaskGetTickCount();
	while (delay + 1000 > xTaskGetTickCount())
		;
	RIT128x96x4Clear();

	if (buttonSelected == NULL) {
		buttonSelected = buttonListRoot;
		buttonSelected->border = pucBorderSelected;
	}

	if (textBoxListSelected == NULL) {
		textBoxListSelected = textBoxListRoot;
		goDrawTextBoxes();
	}

	goDrawButtons();
	goDrawTextBoxes();

	RIT128x96x4StringDraw("", 0, 100, 0x0);
	vTaskSuspend(xGraphTaskHandle);

	while (1) {
		/* Wait for a message to arrive */
		while (xQueueReceive( xGraphCommandQueue, &xCommMessage, 0)) {
			if (xCommMessage.key == BUTTON_SELECT) {
				vTextBoxSetValues();
			} else {
				if (xCommMessage.key == BUTTON_UP) {
					textBoxListSelected = goGetPrevTextBox(textBoxListSelected);
					goDrawTextBoxes();
				} else if (xCommMessage.key == BUTTON_DOWN) {
					textBoxListSelected = goGetNextTextBox(textBoxListSelected);
					goDrawTextBoxes();
				} else if (xCommMessage.key == BUTTON_RIGHT) {
					vTextBoxIncrement(NULL);
				} else if (xCommMessage.key == BUTTON_LEFT) {
					vTextBoxDecrement(NULL);
				} else if (xCommMessage.key == RELOAD) {
					vTextBoxGetValues();
					goDrawTextBoxes();
				}
			}
		}

		while (xQueueReceive( xGraphQueue, &xMessage, 0)) {

			RIT128x96x4StringDraw("                       ", 10, 85, 0);
			RIT128x96x4StringDraw(xMessage.msg, 10, 85, 10);
		}

		vTaskSuspend(xGraphTaskHandle);

	}
}

void goDrawBorder(int height_, int width_, int left, int top,
		unsigned const char * type) {

	unsigned char * pucImage;
	unsigned char * pucImageDraw;

	int i;
	int height = height_ + 4;
	int width = width_ + 4;

	pucImage = (unsigned char *) pvPortMalloc(height * width);
	pucImageDraw = (unsigned char *) pvPortMalloc(height * width / 2);

	for (i = 0; i < width * height; i++) {
		pucImage[i] = 0x00;
	}

	for (i = 0; i < width; i++) {
		pucImage[i] = type[0];
		pucImage[(width * (height - 1)) + i] = type[0];
	}

	for (i = 1; i < height - 1; i++) {
		pucImage[i * width] = type[1];
		pucImage[i * width + width - 1] = type[1];
	}

	for (i = 1; i < width - 1; i++) {
		pucImage[width + i] = type[2];
		pucImage[(width * (height - 2)) + i] = type[2];
	}

	for (i = 2; i < height - 2; i++) {
		pucImage[i * width + 1] = type[3];
		pucImage[i * width + width - 2] = type[3];
	}

	for (i = 0; i < width * height / 2; i++) {
		pucImageDraw[i] = (pucImage[2 * i] << 4) | (pucImage[2 * i + 1] & 0x0F);
	}

	RIT128x96x4ImageDraw(pucImageDraw, left - 2, top - 2, width, height);

	vPortFree(pucImage);
	vPortFree(pucImageDraw);

}

void goPortEIntHandler(void) {
	xGraphCommandMessage xCmdMessage;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if (xTaskGetTickCount() > xTicksLast + 200) {

		xTicksLast = xTaskGetTickCount();

		long stat = GPIOPinIntStatus(GPIO_PORTE_BASE, true);

		if (stat == UP) {
			xCmdMessage.key = BUTTON_UP;
		} else if (stat == DOWN) {
			xCmdMessage.key = BUTTON_DOWN;
		} else if (stat == LEFT) {
			xCmdMessage.key = BUTTON_LEFT;
		} else if (stat == RIGHT) {
			xCmdMessage.key = BUTTON_RIGHT;
		}

		if (stat == UP || stat == DOWN || stat == LEFT || stat == RIGHT) {
			xQueueSendFromISR(xGraphCommandQueue, &xCmdMessage, &xHigherPriorityTaskWoken);
			xTaskResumeFromISR(xGraphTaskHandle);
		}

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, !GPIOPinRead(GPIO_PORTF_BASE,
				GPIO_PIN_0));
	}
	GPIOPinIntClear(GPIO_PORTE_BASE, (UP | DOWN | LEFT | RIGHT));
}

void goPortFIntHandler(void) {
	xGraphCommandMessage xCmdMessage;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if (xTaskGetTickCount() > xTicksLast + 200) {

		xTicksLast = xTaskGetTickCount();

		if (GPIOPinIntStatus(GPIO_PORTF_BASE, true) == SELECT) {
			xCmdMessage.key = BUTTON_SELECT;
			xQueueSendFromISR(xGraphCommandQueue, &xCmdMessage, &xHigherPriorityTaskWoken);
			xTaskResumeFromISR(xGraphTaskHandle);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, !GPIOPinRead(
					GPIO_PORTF_BASE, GPIO_PIN_0));
		}
	}

	GPIOPinIntClear(GPIO_PORTF_BASE, SELECT);
}

void goDisplaySleep(void) {
	RIT128x96x4Clear();
	vTaskSuspend(xGraphTaskHandle);
}
