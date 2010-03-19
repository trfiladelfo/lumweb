/**
 * \addtogroup Tags
 * @{
 *
 * \file SavedParams.c
 * \author Anziner, Hahn
 * \brief Routines for the SavedParams tag
 *
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/io.h"

#include "taglib/tags/SavedParams.h"

/**
 *
 * prints the last set values/params
 *
*/
void io_print_saved_params(char * pcBuf, int iBufLen, SSIParam *param) {
	//int i;
	if (paramValueLen == -1) {
		snprintf(pcBuf, iBufLen, "Keine Parameter gesetzt");
	} else {
	/*	for (i = 0; i <= paramValueLen; i++) {
#if DEBUG_SSI
			printf("io_print_saved_params: valueSet=%s, paramSet=%s \n",
					*(valuesSet + i), *(paramsSet + i));
#endif
		}
*/
		snprintf(pcBuf, iBufLen, "%d Parameter gesetzt", paramValueLen + 1);
	}
}


