
#ifndef __TAGLIB_H
#define __TAGLIB_H

#include "FreeRTOS.h"
#include "hw_types.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"

static const char * const g_pcConfigTags[] = { "DateTime", /// SSI_INDEX_DATEANDTIME
		"IntegerInputField", /// SSI_INDEX_INTEGERINPUTFIELD
		"SubmitInputField", /// SSI_INDEX_SUBMITINPUTFIELD
		"SavedParams",		///SSI_INDEX_SAVEDPARAMS
		"CheckboxInputField", /// SSI_INDEX_CHECKBOXINPUTFIELD
		"Hyperlink", /// SSI_INDEX_HYPERLINK
		"Titel", /// SSI_INDEX_TITLE
		"Group", /// SSI_INDEX_GROUP
		"TimeInputField", /// SSI_INDEX_TIMEINPUTFIELD
		"FloatInputField" /// SSI_INDEX_FLOATINPUTFIELD

};

#define TAG_INDEX_DATEANDTIME  			0
#define TAG_INDEX_INTEGERINPUTFIELD    	        1
#define TAG_INDEX_SUBMITINPUTFIELD     	        2
#define TAG_INDEX_SAVEDPARAMS    		3
#define TAG_INDEX_CHECKBOXINPUTFIELD            4
#define TAG_INDEX_HYPERLINK			5
#define TAG_INDEX_TITEL				6
#define TAG_INDEX_GROUP				7
#define TAG_INDEX_TIMEINPUTFIELD	        8
#define TAG_INDEX_FLOATINPUTFIELD	        9

#define NUM_CONFIG_TAGS     (sizeof(g_pcConfigTags) / sizeof (char *))

typedef struct __taglibStruct {
	char* tagname;

	void (* renderSSI )(char * pcBuf, int iBufLen, pSSIParam *params);

	void (* onLoad)(char*, int, void* basicDisplayLine);
	tWidget (* onDisplay) (void* basicDisplayLine, int);
	void (* onEditValue) (void* basicDisplayLine);
	void (* onDestroy) (void* basicDisplayLine);
	char (* strFormatter) (void* basicDisplayLine);

	void* userSpace;
} taglib;

char* pcGetParamFromString(char* str, char* search);
void vInserIntoList(void* basicDisplayLine);
void vCreateNewEntity(taglib *type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment);



#endif // End of Header


