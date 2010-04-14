/**
 * \addtogroup Tags
 * @{
 *
 * \file DefaultTags.c
 * \author Anziner, Hahn
 * \brief Routines for dummy default tag
 *
 */

#include <string.h>
#include <stdio.h>

#include "FreeRTOS.h"

#include "setup.h"

#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vDummyOnLoadPtr(char* param, int len, void* this)
{
	;
}

tWidget* xDummyOnDisplay(void* this, int row)
{
	return ((basicDisplayLine*) this)->valueWidget;
}

void vDummyOnEditValue(void* this)
{
	;
}
void vDummyOnDestroy(void* this)
{
	;
}

char* pcDummyStrFormatter(void* this)
{
	return ((basicDisplayLine*) this)->strValue;
}

void vDummyRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

