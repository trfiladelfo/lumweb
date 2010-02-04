/**
 * \addtogroup Graphic
 * @{
 *
 * \file touchAction.h
 * \author Anziner, Hahn
 * \brief
 *
*/


#ifndef TOUCHACTIONS_H_
#define TOUCHACTIONS_H_

#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"

#include "graphic/graphicClientBuilder.h"

void onCheckboxClick(tWidget *pWidget, unsigned long bSelected);
void loadWeb(tWidget *pWidget);
void insertClientEntity(xClientEntity* toInsert);
void increase(tWidget *pWidget);
void decrease(tWidget *pWidget);
void backPage(tWidget *pWidget);
void continuePage(tWidget *pWidget);
void vLoadNewPage(tWidget *pWidget);

#endif /* TOUCHACTIONS_H_ */
