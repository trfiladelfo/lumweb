/*
 * touchActions.h
 *
 *  Created on: 18.01.2010
 *      Author: root
 */

#ifndef TOUCHACTIONS_H_
#define TOUCHACTIONS_H_

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"

#include "graphic/graphicWebClient.h"

void onCheckboxClick(tWidget *pWidget, unsigned long bSelected);
void loadWeb(tWidget *pWidget);
void insertGWC(gwcRow* toInsert);
void increase(tWidget *pWidget);
void decrease(tWidget *pWidget);
void backPage(tWidget *pWidget);
void continuePage(tWidget *pWidget);

#endif /* TOUCHACTIONS_H_ */
