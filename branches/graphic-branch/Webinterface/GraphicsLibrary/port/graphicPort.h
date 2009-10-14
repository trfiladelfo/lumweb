/*
 * graphicPort.h
 *
 *  Created on: 14.10.2009
 *      Author: root
 */

#ifndef GRAPHICPORT_H_
#define GRAPHICPORT_H_

#include "rit128x96x4.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 96

#define FONT_SIZE 	8
#define FONT_WIDTH 	6

void vGraphicClear(void);
void vGraphicStringDraw(const char *pcStr, unsigned long ulX,
		unsigned long ulY, unsigned char ucLevel);
void vGraphicImageDraw(const unsigned char *pucImage, unsigned long ulX,
		unsigned long ulY, unsigned long ulWidth, unsigned long ulHeight);
void vGraphicInit(unsigned long ulFrequency);

#endif /* GRAPHICPORT_H_ */
