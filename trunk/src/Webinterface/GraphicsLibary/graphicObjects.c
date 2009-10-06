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

#include "graphicObjects.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "gpio.h"
#include "sysctl.h"

/*
 * Prototypes
 */
pgoButton goGetNextButton(pgoButton btn);
pgoButton goGetLastButton(void);
pgoButton goGetPrevButton(pgoButton btn);
pgoButton goGetFirstButton(void);

void goInsertButton(pgoButton btn);
void goRemoveButton(pgoButton btn);

void goDrawEmptyButton(int height, int width, int left, int top,
		unsigned const char * type);

/*
 * Statusvariables
 */
char isAlreadyInitilized = 0;

/*
 * A Query to
 */
pgoButton buttonListRoot;
pgoButton buttonListLast;
pgoButton buttonSelected;

/**
 * Initializes the Display to Draw the Buttons
 */
void goInit() {
	if (!isAlreadyInitilized) {

		SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN
				| SYSCTL_XTAL_8MHZ);

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

		GPIOPadConfigSet(GPIO_PORTE_BASE, DOWN, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, DOWN, GPIO_DIR_MODE_IN);

		GPIOPadConfigSet(GPIO_PORTE_BASE, LEFT, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD_WPU);
		GPIODirModeSet(GPIO_PORTE_BASE, LEFT, GPIO_DIR_MODE_IN);

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

/**
 * Draws a simple Button with Labeltext
 *
 * returns a structure from the Button
 */
pgoButton goNewButton(int height, int width, int left, int top,
		char unsigned *value) {

	// Allocate RAM for the Button
	//struct goButton *btn = (struct goButton *) pvPortMalloc(sizeof (goButton));
	pgoButton btn = (pgoButton) pvPortMalloc(sizeof(struct goButton));

	btn->height = height;
	btn->width = width;
	btn->left = left;
	btn->top = top;
	btn->value = value;

	goInsertButton(btn);

	return btn;
}

// only pics with odd Pixel Numbers
void goDrawEmptyButton(int height_, int width_, int left, int top,
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

	for (i = 0; i < width; i++) { // ok
		pucImage[i] = type[0];
		pucImage[(width * (height - 1)) + i] = type[0];
	}

	for (i = 1; i < height - 1; i++) {
		pucImage[i * width] = type[1];
		pucImage[i * width + width - 1] = type[1];
	}

	for (i = 1; i < width - 1; i++) { // ok
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

/*
 * Draws a Button to the Screen
 */
void goDrawButton(pgoButton btn, unsigned const char* type) {
	if (type == NULL) {
		type = pucButtonNormal;
	}
	goDrawEmptyButton(btn->height, btn->width, btn->left, btn->top, type);

	RIT128x96x4ImageDraw(btn->value, btn->left, btn->top, btn->width,
			btn->height);
}

void goDrawButtons(void) {
	pgoButton akt = buttonListRoot;
	while (akt != NULL) {

		goDrawButton(akt, NULL);

		akt = akt->next;
	}
}

/*
 * Frees the allocated Memory
 */
void goDeleteButton(pgoButton btn) {
	pgoButton toDelete;
	toDelete = btn;
	btn = btn->next;
	vPortFree(toDelete);
}

/*
 * Returns the next Button, null if there isn't one
 */
pgoButton goGetNextButton(pgoButton btn) {
	return btn->next;
}

/*
 * Returns the Last Button, null if List ist empty
 */
pgoButton goGetLastButton(void) {
	return buttonListLast;
}

/*
 * Returns the Button before, null if empty or first
 */
pgoButton goGetPrevButton(pgoButton btn) {
	pgoButton akt = buttonListRoot;
	while (akt->next != NULL && akt->next != btn) {
		akt = akt->next;
	}
	return akt;
}

/*
 * Returns the First Button, null if empty
 */
pgoButton goGetFirstButton(void) {
	return buttonListRoot;
}

/*
 * Inserts the Button into the List
 */
void goInsertButton(pgoButton btn) {
	if (buttonListRoot == NULL) {
		buttonListRoot = btn;
	} else {
		buttonListLast->next = btn;
	}
	buttonListLast = btn;
}

/*
 *
 */
void goStartListener(void) {
	tBoolean pressed = false;
	xGRAPHMessage xMessage;

	while (1) {


		/* Wait for a message to arrive */
		if(xQueueReceive( xGRAPHQueue, &xMessage, ( portTickType ) 10 )){
			/* Print received message */
			RIT128x96x4StringDraw(xMessage.msg, 10, 85, 10);
		}

		//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, !GPIOPinRead(GPIO_PORTE_BASE,
		//		UP));
		if (buttonSelected == NULL) {
			buttonSelected = buttonListRoot;
			goDrawButton(buttonSelected, pucButtonSelected);

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
									goDrawButton(buttonSelected, pucButtonNormal);
									buttonSelected = goGetPrevButton(buttonSelected);
									if (buttonSelected == NULL) {
										buttonSelected = buttonListLast;
									}
									if (buttonSelected != NULL) {
										goDrawButton(buttonSelected, pucButtonSelected);
									}
								}
								pressed = true;
				pressed = true;
			}
			// RIGHT
			if (!GPIOPinRead(GPIO_PORTE_BASE, RIGHT)) {
				if (buttonSelected != NULL) {
					goDrawButton(buttonSelected, pucButtonNormal);
					buttonSelected = buttonSelected->next;
					if (buttonSelected != NULL) {
						goDrawButton(buttonSelected, pucButtonSelected);
					}
				}
				pressed = true;
			}

			// SELECT
			if (!GPIOPinRead(GPIO_PORTF_BASE, SELECT)) {

				goDrawButton(buttonSelected, pucButtonClicked);
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
								goDrawButton(buttonSelected, pucButtonSelected);
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
