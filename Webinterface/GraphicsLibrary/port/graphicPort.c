/*
 * graphicPort.c
 *
 *  Created on: 14.10.2009
 *      Author: anzinger
 */

void vGraphicClear(void) {
	RIT128x96x4Clear();
}
void vGraphicStringDraw(const char *pcStr, unsigned long ulX,
		unsigned long ulY, unsigned char ucLevel) {
	RIT128x96x4StringDraw(pcStr, ulX, ulY, ucLevel);
}
void vGraphicImageDraw(const unsigned char *pucImage, unsigned long ulX,
		unsigned long ulY, unsigned long ulWidth, unsigned long ulHeight) {
	RIT128x96x4ImageDraw(pucImage, ulX, ulY, ulWidth, ulHeight);
}
void vGraphicInit(unsigned long ulFrequency) {
	RIT128x96x4Init(ulFrequency);
}
