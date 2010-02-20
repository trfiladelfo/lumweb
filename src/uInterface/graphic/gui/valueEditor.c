/*
 * valueEditor.c
 *
 *  Created on: 20.02.2010
 *      Author: root
 */

#include "FreeRTOS.h"
#include "valueEditor.h"

#include "graphic/gui/displayBasics.h"
#include "graphic/gui/editorStyle.h"

#include "grlib/grlib.h"
#include "grlib/pushbutton.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"

tPushButtonWidget xIncrease[2];
tPushButtonWidget xDecrease[2];
tPushButtonWidget xCancel;
tPushButtonWidget xSubmit;

char* valueBuffer[2][4];

void vBuildEditor (basicDisplayLine* akt) {

}
