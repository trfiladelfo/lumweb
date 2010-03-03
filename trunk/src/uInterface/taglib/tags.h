/*
 * tags.h
 *
 *  Created on: 02.03.2010
 *      Author: root
 */

#ifndef TAGS_H_
#define TAGS_H_

#include "graphic/gui/displayBasics.h"

// DUMMY FUNCTIONS
void vDummyOnLoadPtr (char*, int, void* this);
tWidget* vDummyOnDisplayPtr ( void* this, int);
void vDummyOnEditValuePtr ( void* this);
void vDummyOnDestroyPtr ( void* this);
char* vDummyStrFormatter (void* this);

// PRODUCTIVE FUNCTIONS

static const taglib* xTagList[] = {
		{0,	vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // DATETIME
		{1, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // INTEGERINPUT
		{2, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // SUBMIT
		{3, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // SAVED PARAMS
		{4, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // CHECKBOX
		{5, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // HYPERLINK
		{6, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // TITLE
		{7, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // GROUP
		{8, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}, // TIMEINPUT
		{9, vDummyOnLoadPtr, vDummyOnDisplayPtr, vDummyOnEditValuePtr, vDummyOnDestroyPtr, vDummyStrFormatter, NULL}  // FLOATINPUT
};

#endif /* TAGS_H_ */
