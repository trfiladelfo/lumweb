/*
 * graphicWebClient.h
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#ifndef GRAPHICCLIENTBUILDER_H_
#define GRAPHICCLIENTBUILDER_H_

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"

#include "lwip/ip_addr.h"

extern int aktPage;

#define GWC_LABEL   		0b00000001
#define GWC_NUMERIC 		0b00000010
#define GWC_BOOLEAN 		0b00000100
#define GWC_LINK    		0b00001000
#define GWC_VALUE_MIN 		0b00010000
#define GWC_VALUE_MAX 		0b00100000
#define GWC_SUBMIT		 	0b01000000
#define GWC_HYPERLINK		0b10000000

typedef struct __ClientEntity  {
	char *id;
	char *name;
	int value;
	int minValue;
	int maxValue;
	int increment;
	char decimal;
	char* stringValue;
	char status;
	tCanvasWidget *nameLabel;
	tCanvasWidget *valueLabel;
	tPushButtonWidget *decrease;
	tPushButtonWidget *increase;
	tPushButtonWidget *actionButton;
	tCheckBoxWidget *checkbox;
	struct __ClientEntity* next;
}xClientEntity;

xClientEntity *xClientRoot;

extern int aktPage;

void vLoadWebPage (char *uri);
void vLoadMenu    (void);

char* getElementType(char * str, char *buff);
char* getValueForParamName(char* str, char* search, char* retValue);

void vAddRow(char* id, char* name, long value, long min, long max, long inc, long decimal, char status);
void vAddActionButton (char* name, char* link, char status);

void vDrawClientEntity(void);
void vDestroyClientEntities(void);

#endif /* GRAPHICWEBCLIENT_H_ */
