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
	xTagList[0].tagname = "IntegerInputField";
	xTagList[0].renderSSI = vIntegerRenderSSI;
	xTagList[0].onLoad = vIntegerOnLoad;
	xTagList[0].onDisplay = xIntegerOnDisplay;
	xTagList[0].onEditValue = vDummyOnEditValue;
	xTagList[0].onDestroy = vDummyOnDestroy;
	xTagList[0].strFormatter = pcIntegerStrFormatter;
	xTagList[0].userSpace = NULL;

	xTagList[1].tagindex = 1;
	xTagList[1].tagname = "SubmitInputField";
	xTagList[1].renderSSI = vSubmitButtonRenderSSI;
	xTagList[1].onLoad = vSubmitButtonOnLoad;
	xTagList[1].onDisplay = NULL;
	xTagList[1].onEditValue = NULL;
	xTagList[1].onDestroy = vDummyOnDestroy;
	xTagList[1].strFormatter = NULL;
	xTagList[1].userSpace = NULL;

	xTagList[2].tagindex = 2;
	xTagList[2].tagname = "SavedParams";
	xTagList[2].renderSSI = vSavedParamsRenderSSI;
	xTagList[2].onLoad = NULL;
	xTagList[2].onDisplay = NULL;
	xTagList[2].onEditValue = NULL;
	xTagList[2].onDestroy = NULL;
	xTagList[2].strFormatter = NULL;
	xTagList[2].userSpace = NULL;

	xTagList[3].tagindex = 3;
	xTagList[3].tagname = "CheckboxInputField";
	xTagList[3].renderSSI = vCheckboxRenderSSI;
	xTagList[3].onLoad = vCheckboxOnLoad;
	xTagList[3].onDisplay = xCheckboxOnDisplay;
	xTagList[3].onEditValue = vDummyOnEditValue;
	xTagList[3].onDestroy = vDummyOnDestroy;
	xTagList[3].strFormatter = pcDummyStrFormatter;
	xTagList[3].userSpace = NULL;

	xTagList[4].tagindex = 4;
	xTagList[4].tagname = "Hyperlink";
	xTagList[4].renderSSI = vHyperlinkRenderSSI;
	xTagList[4].onLoad = vHyperlinkOnLoad;
	xTagList[4].onDisplay = xHyperlinkOnDisplay;
	xTagList[4].onEditValue = NULL;
	xTagList[4].onDestroy = vDummyOnDestroy;
	xTagList[4].strFormatter = NULL;
	xTagList[4].userSpace = NULL;

	xTagList[5].tagindex = 5;
	xTagList[5].tagname = "Titel";
	xTagList[5].renderSSI = vTitleRenderSSI;
	xTagList[5].onLoad = vTitleOnLoad;
	xTagList[5].onDisplay = NULL;
	xTagList[5].onEditValue = NULL;
	xTagList[5].onDestroy = vDummyOnDestroy;
	xTagList[5].strFormatter = NULL;
	xTagList[5].userSpace = NULL;

	xTagList[6].tagindex = 6;
	xTagList[6].tagname = "Group";
	xTagList[6].renderSSI = vGroupRenderSSI;
	xTagList[6].onLoad = vGroupOnLoad;
	xTagList[6].onDisplay = xDummyOnDisplay;
	xTagList[6].onEditValue = NULL;
	xTagList[6].onDestroy = vDummyOnDestroy;
	xTagList[6].strFormatter = NULL;
	xTagList[6].userSpace = NULL;

	xTagList[7].tagindex = 7;
	xTagList[7].tagname = "TimeInputField";
	xTagList[7].renderSSI = vTimeRenderSSI;
	xTagList[7].onLoad = vTimeOnLoad;
	xTagList[7].onDisplay = xTimeOnDisplay;
	xTagList[7].onEditValue = vDummyOnEditValue;
	xTagList[7].onDestroy = vDummyOnDestroy;
	xTagList[7].strFormatter = pcTimeStrFormatter;
	xTagList[7].userSpace = NULL;

	xTagList[8].tagindex = 8;
	xTagList[8].tagname = "FloatInputField";
	xTagList[8].renderSSI = vFloatRenderSSI;
	xTagList[8].onLoad = vFloatOnLoad;
	xTagList[8].onDisplay = xFloatOnDisplay;
	xTagList[8].onEditValue = vDummyOnEditValue;
	xTagList[8].onDestroy = vDummyOnDestroy;
	xTagList[8].strFormatter = pcFloatStrFormatter;
	xTagList[8].userSpace = NULL;
}
