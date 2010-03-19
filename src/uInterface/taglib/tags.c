/*
 * tags.c
 *
 *  Created on: 19.03.2010
 *      Author: root
 */

#include "taglib/tags.h"
#include "taglib/taglib.h"

void vInitTagLibrary(void)
{
	xTagList[0].tagindex = 0;
	xTagList[0].tagname = "DateTime";
	xTagList[0].renderSSI = NULL;
	xTagList[0].onLoad = vDummyOnLoadPtr;
	xTagList[0].onDisplay = vDummyOnDisplayPtr;
	xTagList[0].onEditValue = vDummyOnEditValuePtr;
	xTagList[0].onDestroy = vDummyOnDestroyPtr;
	xTagList[0].strFormatter = vDummyStrFormatter;
	xTagList[0].userSpace = NULL;

	xTagList[1].tagindex = 1;
	xTagList[1].tagname = "IntegerInputField";
	xTagList[1].renderSSI =  io_get_submit_input_button;
	xTagList[1].onLoad = vDummyOnLoadPtr;
	xTagList[1].onDisplay = vDummyOnDisplayPtr;
	xTagList[1].onEditValue = vDummyOnEditValuePtr;
	xTagList[1].onDestroy = vDummyOnDestroyPtr;
	xTagList[1].strFormatter = vDummyStrFormatter;
	xTagList[1].userSpace = NULL;

	xTagList[2].tagindex = 2;
	xTagList[2].tagname = "SubmitInputField";
	xTagList[2].renderSSI = io_print_saved_params;
	xTagList[2].onLoad = vDummyOnLoadPtr;
	xTagList[2].onDisplay = vDummyOnDisplayPtr;
	xTagList[2].onEditValue = vDummyOnEditValuePtr;
	xTagList[2].onDestroy = vDummyOnDestroyPtr;
	xTagList[2].strFormatter = vDummyStrFormatter;
	xTagList[2].userSpace = NULL;

	xTagList[3].tagindex = 3;
	xTagList[3].tagname = "SavedParams";
	xTagList[3].renderSSI = NULL;
	xTagList[3].onLoad = vDummyOnLoadPtr;
	xTagList[3].onDisplay = vDummyOnDisplayPtr;
	xTagList[3].onEditValue = vDummyOnEditValuePtr;
	xTagList[3].onDestroy = vDummyOnDestroyPtr;
	xTagList[3].strFormatter = vDummyStrFormatter;
	xTagList[3].userSpace = NULL;

	xTagList[4].tagindex = 4;
	xTagList[4].tagname = "CheckboxInputField";
	xTagList[4].renderSSI = NULL;
	xTagList[4].onLoad = vDummyOnLoadPtr;
	xTagList[4].onDisplay = vDummyOnDisplayPtr;
	xTagList[4].onEditValue = vDummyOnEditValuePtr;
	xTagList[4].onDestroy = vDummyOnDestroyPtr;
	xTagList[4].strFormatter = vDummyStrFormatter;
	xTagList[4].userSpace = NULL;

	xTagList[5].tagindex = 5;
	xTagList[5].tagname = "Hyperlink";
	xTagList[5].renderSSI = io_get_hyperlink;
	xTagList[5].onLoad = vDummyOnLoadPtr;
	xTagList[5].onDisplay = vDummyOnDisplayPtr;
	xTagList[5].onEditValue = vDummyOnEditValuePtr;
	xTagList[5].onDestroy = vDummyOnDestroyPtr;
	xTagList[5].strFormatter = vDummyStrFormatter;
	xTagList[5].userSpace = NULL;

	xTagList[6].tagindex = 6;
	xTagList[6].tagname = "Titel";
	xTagList[6].renderSSI = io_get_titel;
	xTagList[6].onLoad = vDummyOnLoadPtr;
	xTagList[6].onDisplay = vDummyOnDisplayPtr;
	xTagList[6].onEditValue = vDummyOnEditValuePtr;
	xTagList[6].onDestroy = vDummyOnDestroyPtr;
	xTagList[6].strFormatter = vDummyStrFormatter;
	xTagList[6].userSpace = NULL;

	xTagList[7].tagindex = 7;
	xTagList[7].tagname = "Group";
	xTagList[7].renderSSI = io_get_group;
	xTagList[7].onLoad = vDummyOnLoadPtr;
	xTagList[7].onDisplay = vDummyOnDisplayPtr;
	xTagList[7].onEditValue = vDummyOnEditValuePtr;
	xTagList[7].onDestroy = vDummyOnDestroyPtr;
	xTagList[7].strFormatter = vDummyStrFormatter;
	xTagList[7].userSpace = NULL;

	xTagList[8].tagindex = 8;
	xTagList[8].tagname = "TimeInputField";
	xTagList[8].renderSSI = io_get_time_input_field;
	xTagList[8].onLoad = vDummyOnLoadPtr;
	xTagList[8].onDisplay = vDummyOnDisplayPtr;
	xTagList[8].onEditValue = vDummyOnEditValuePtr;
	xTagList[8].onDestroy = vDummyOnDestroyPtr;
	xTagList[8].strFormatter = vDummyStrFormatter;
	xTagList[8].userSpace = NULL;

	xTagList[9].tagindex = 9;
	xTagList[9].tagname = "FloatInputField";
	xTagList[9].renderSSI = io_get_float_input_field;
	xTagList[9].onLoad = vDummyOnLoadPtr;
	xTagList[9].onDisplay = vDummyOnDisplayPtr;
	xTagList[9].onEditValue = vDummyOnEditValuePtr;
	xTagList[9].onDestroy = vDummyOnDestroyPtr;
	xTagList[9].strFormatter = vDummyStrFormatter;
	xTagList[9].userSpace = NULL;

}
