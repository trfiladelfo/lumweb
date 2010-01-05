/*
 * graphicWebClient.h
 *
 *  Created on: 05.01.2010
 *      Author: root
 */

#ifndef GRAPHICWEBCLIENT_H_
#define GRAPHICWEBCLIENT_H_

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"

#define GWC_LABEL   0b00000001;
#define GWC_NUMERIC 0b00000010;
#define GWC_BOOLEAN 0b00000100;
#define GWC_LINK    0b00001000;

typedef struct  {
	char *id;
	char *name;
	long value;
	tCanvasWidget *nameLabel;
	tCanvasWidget *valueLabel;
	tPushButtonWidget *decrease;
	tPushButtonWidget *increase;
	tCheckBoxWidget *checkbox;
	struct gwcRow* next;
}gwcRow;

gwcRow *root;

void loadMenu (void);

#endif /* GRAPHICWEBCLIENT_H_ */
