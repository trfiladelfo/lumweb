
#include <string.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

void vParseTitle(char* param, int len) {
	char *label;
	label = pcGetParamFromString(param, "label");

	if (label != NULL) {
		xDisplayRoot.title = label;
		vSetTitle(label);
	} else {
#if DEBUG_HTTPC
		printf("vParseTitle: label NULL\n");
#endif
	}
}
