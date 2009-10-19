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
 * graphicObjects.c
 *
 *  Created on: 23.09.2009
 *      Author: anzinger
 *
 * This Libary is used to draw the Graphic Objects to different Display Types.
 *
 * Primary it is made to make selections with the menu buttons (UP, DOWN, LEFT, RIGHT, SELECT)
 *
 * It should also be able to make a touch Interface.
 */

#include <string.h>

#include "graphicObjects.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "gpio.h"
#include "sysctl.h"
#include "interrupt.h"
#include "hw_ints.h"

#include "rit128x96x4.h"

xTaskHandle xGraphicTaskHandler = NULL;

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
		// Configure UP | DOWN | LEFT | RIGHT
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		GPIOPadConfigSet(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT),
				GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT),
				GPIO_DIR_MODE_IN);

		// Configure SELECT
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPadConfigSet(GPIO_PORTF_BASE, SELECT, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTF_BASE, SELECT, GPIO_DIR_MODE_IN);

		GPIOIntTypeSet(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT),
				GPIO_FALLING_EDGE);
		GPIOPinIntEnable(GPIO_PORTE_BASE, (UP | DOWN | RIGHT | LEFT));

		GPIOIntTypeSet(GPIO_PORTE_BASE, SELECT, GPIO_FALLING_EDGE);
		GPIOPinIntEnable(GPIO_PORTF_BASE, SELECT);

		xGraphCommandQueue = xQueueCreate(5, sizeof(xGraphCommandMessage));

		RIT128x96x4Init(1000000);
		RIT128x96x4DisplayOn();

		IntEnable(INT_GPIOE);
		IntEnable(INT_GPIOF);
		IntMasterEnable();

		isAlreadyInitilized = 1;
	}
}

/*
 *
 */
void goStartListener(xTaskHandle handler) {

	char cMessageLast[16];

	xGraphicTaskHandler = handler;

	xGraphCommandMessage xMessage;

	strcpy(cMessageLast, "Kein Befehl\0");
	RIT128x96x4StringDraw(cMessageLast, 10, 85, 10);

	while (1) {
		/* Wait for a message to arrive */
		while (xQueueReceive( xGraphCommandQueue, &xMessage, 0)) {
			/* Print received message */
			RIT128x96x4StringDraw(cMessageLast, 10, 85, 0);
			snprintf(cMessageLast, 10, "Befehl: %d", xMessage.key);
			RIT128x96x4StringDraw(cMessageLast, 10, 85, 10);
		}
		vTaskSuspend(xGraphicTaskHandler);

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
		pucImage[i] = 0xFF;
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

	IntDisable(INT_GPIOE);

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
	xQueueSendFromISR(xGraphCommandQueue, &xCmdMessage, &xHigherPriorityTaskWoken);
	xTaskResumeFromISR(xGraphicTaskHandler);
	GPIOPinIntClear(GPIO_PORTE_BASE, (UP | DOWN | LEFT | RIGHT));
	IntEnable(INT_GPIOE);
}

void goPortFIntHandler(void) {
	xGraphCommandMessage xCmdMessage;
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	IntDisable(INT_GPIOF);
	if (GPIOPinIntStatus(GPIO_PORTF_BASE, true) == SELECT) {
		xCmdMessage.key = BUTTON_SELECT;
	}
	xQueueSendFromISR(xGraphCommandQueue, &xCmdMessage, &xHigherPriorityTaskWoken);
	xTaskResumeFromISR(xGraphicTaskHandler);
	GPIOPinIntClear(GPIO_PORTF_BASE, SELECT);
	IntEnable(INT_GPIOF);
}
