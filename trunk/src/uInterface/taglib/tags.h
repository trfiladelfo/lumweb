/*
 * tags.h
 *
 *  Created on: 02.03.2010
 *      Author: root
 */

#ifndef TAGS_H_
#define TAGS_H_

#include "graphic/gui/displayBasics.h"
#include "taglib.h"

#include "taglib/tags/CheckboxInputField.h"
#include "taglib/tags/FloatInputField.h"
#include "taglib/tags/Group.h"
#include "taglib/tags/Hyperlink.h"
#include "taglib/tags/IntegerInputField.h"
#include "taglib/tags/SavedParams.h"
#include "taglib/tags/SubmitInputField.h"
#include "taglib/tags/TimeInputField.h"
#include "taglib/tags/Titel.h"
#include "taglib/tags/DefaultTags.h"


#define NUM_CONFIG_TAGS    10

taglib xTagList[NUM_CONFIG_TAGS];

#define TAG_INDEX_DATEANDTIME                   0
#define TAG_INDEX_INTEGERINPUTFIELD             1
#define TAG_INDEX_SUBMITINPUTFIELD              2
#define TAG_INDEX_SAVEDPARAMS                   3
#define TAG_INDEX_CHECKBOXINPUTFIELD            4
#define TAG_INDEX_HYPERLINK                     5
#define TAG_INDEX_TITEL                         6
#define TAG_INDEX_GROUP                         7
#define TAG_INDEX_TIMEINPUTFIELD                8
#define TAG_INDEX_FLOATINPUTFIELD               9

void vInitTagLibrary(void);


#endif /* TAGS_H_ */
