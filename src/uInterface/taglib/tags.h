/**
 * \addtogroup TagLib
 * @{
 *
 * \author Anziner, Hahn
 * \brief
 *
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

#define NUM_CONFIG_TAGS    9

taglib xTagList[NUM_CONFIG_TAGS];

#define TAG_INDEX_INTEGERINPUTFIELD             0
#define TAG_INDEX_SUBMITINPUTFIELD              1
#define TAG_INDEX_SAVEDPARAMS                   2
#define TAG_INDEX_CHECKBOXINPUTFIELD            3
#define TAG_INDEX_HYPERLINK                     4
#define TAG_INDEX_TITEL                         5
#define TAG_INDEX_GROUP                         6
#define TAG_INDEX_TIMEINPUTFIELD                7
#define TAG_INDEX_FLOATINPUTFIELD               8

void vInitTagLibrary(void);

#endif /* TAGS_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

