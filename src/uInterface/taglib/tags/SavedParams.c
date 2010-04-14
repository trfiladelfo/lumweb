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
#include "setup.h"

#include "taglib/tags.h"

#include "ethernet/httpd/cgi/io.h"

#include "taglib/tags/SavedParams.h"

/**
 *
 * prints the last set values/params
 *
 */
void vSavedParamsRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params)
{
	if (paramValueLen == -1)
	{
		snprintf(pcBuf, iBufLen, "Keine Parameter gesetzt");
	}
	else
	{
		snprintf(pcBuf, iBufLen, "%d Parameter gesetzt", paramValueLen + 1);
	}
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


