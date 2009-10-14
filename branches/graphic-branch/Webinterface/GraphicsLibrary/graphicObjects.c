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

#include "rit128x96x4.h"

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
		// Configure the GPIO that drives the on-board LED.
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPadConfigSet(GPIO_PORTF_BASE, SELECT, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTF_BASE, SELECT, GPIO_DIR_MODE_IN);

		// Configure UP Button as INPUT
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		GPIOPadConfigSet(GPIO_PORTE_BASE, UP, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, UP, GPIO_DIR_MODE_IN);
		// Configure DOWN Button as INPUT
		GPIOPadConfigSet(GPIO_PORTE_BASE, DOWN, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, DOWN, GPIO_DIR_MODE_IN);
		// Configure LEFT Button as INPUT
		GPIOPadConfigSet(GPIO_PORTE_BASE, LEFT, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, LEFT, GPIO_DIR_MODE_IN);
		// Configure RIGHT Button as INPUT
		GPIOPadConfigSet(GPIO_PORTE_BASE, RIGHT, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, RIGHT, GPIO_DIR_MODE_IN);

		// Configure the GPIO that drives the on-board LED.
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

		RIT128x96x4Init(1000000);
		RIT128x96x4DisplayOn();

		isAlreadyInitilized = 1;
	}
}

/*
 *
 */
void goStartListener(void) {
	char i = 0;
	tBoolean pressed = false;
	char * ucMessageLast;
	xGRAPHMessage xMessage;

	while (1) {
		if (i = 255) {
			goDrawButtons();
			i = 0;
		}
		i++;

		/* Wait for a message to arrive */
		if (xQueueReceive( xGRAPHQueue, &xMessage, ( portTickType ) 10 )) {
			/* Print received message */
			if (strcmp((const char *) ucMessageLast,
					(const char *) xMessage.msg) != 0) {
				RIT128x96x4StringDraw((const char *) ucMessageLast, 10, 85, 0);
				RIT128x96x4StringDraw((const char *) xMessage.msg, 10, 85, 10);
				strcpy(ucMessageLast, xMessage.msg);
			}
		}

		if (buttonSelected == NULL) {
			buttonSelected = buttonListRoot;
			goDrawButton(buttonSelected, pucBorderSelected);

		}

		if (!pressed) {

			// UP
			if (!GPIOPinRead(GPIO_PORTE_BASE, UP)) {
				pressed = true;
			}
			// DOWN
			if (!GPIOPinRead(GPIO_PORTE_BASE, DOWN)) {
				pressed = true;
			}
			// LEFT
			if (!GPIOPinRead(GPIO_PORTE_BASE, LEFT)) {
				if (buttonSelected != NULL) {
					goDrawButton(buttonSelected, pucBorderNormal);
					buttonSelected = goGetPrevButton(buttonSelected);
					if (buttonSelected == NULL) {
						buttonSelected = buttonListLast;
					}
					if (buttonSelected != NULL) {
						goDrawButton(buttonSelected, pucBorderSelected);
					}
				}

				pressed = true;
			}
			// RIGHT
			if (!GPIOPinRead(GPIO_PORTE_BASE, RIGHT)) {
				if (buttonSelected != NULL) {
					goDrawButton(buttonSelected, pucBorderNormal);
					buttonSelected = buttonSelected->next;
					if (buttonSelected != NULL) {
						goDrawButton(buttonSelected, pucBorderSelected);
					}
				}

				pressed = true;
			}

			// SELECT
			if (!GPIOPinRead(GPIO_PORTF_BASE, SELECT)) {

				goDrawButton(buttonSelected, pucBorderClicked);

				if (buttonSelected->selectAction != NULL) {
					buttonSelected->selectAction();
				}

				pressed = true;
			}

			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
		} else {
			if (GPIOPinRead(GPIO_PORTE_BASE, UP)) {
				if (GPIOPinRead(GPIO_PORTE_BASE, DOWN)) {
					if (GPIOPinRead(GPIO_PORTE_BASE, LEFT)) {
						if (GPIOPinRead(GPIO_PORTE_BASE, RIGHT)) {
							if (GPIOPinRead(GPIO_PORTF_BASE, SELECT)) {
								pressed = false;
								goDrawButton(buttonSelected, pucBorderSelected);
								GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);
							}
						}
					}
				}
			}
		}

		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, !GPIOPinRead(GPIO_PORTF_BASE,
				SELECT));
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
