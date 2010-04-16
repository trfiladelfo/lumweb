/**
 * \addtogroup TagLib
 * @{
 *
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "taglib.h"

#include "setup.h"
#include "utils.h"

#include "graphic/gui/displayBasics.h"

char* pcGetParamFromString(char* str, char* search)
{
	char *buffer, *retValue = NULL;
	int i, j, len;

#if DEBUG_HTTPC
	printf("pcGetParamFromString str=%s, search=%s,\n", str, search);
#endif

	buffer = pcStrstr(str, search);
	if (buffer != NULL)
	{

		buffer = pcStrstr(buffer, "=\"");
		buffer += 2;

		for (len = 0; buffer[len] != '\"'; len++)
			;

		retValue = pvPortMalloc((len + 1) * sizeof(char));

		j = 0;
		for (i = 0; i < len; i++)
		{
			retValue[j] = buffer[i];
			j++;
		}
		retValue[j] = 0;
	}

	return retValue;
}

void vInserIntoList(void *toInsert)
{

	basicDisplayLine *akt = xDisplayRoot.entities;

	if (toInsert != NULL)
	{

		if (akt == NULL || xDisplayRoot.displayEntities == false)
		{
			xDisplayRoot.entities = toInsert;
			xDisplayRoot.displayEntities = true;

#if DEBUG_HTTPC
			printf("vInsertIntoList: root\n");
#endif
		}
		else
		{
			while (akt->next != NULL)
			{
				akt = akt->next;
			}
			akt->next = toInsert;

#if DEBUG_HTTPC
			printf("vInsertIntoList: at the End\n");
#endif

			xDisplayRoot.displayEntities = true;
		}
	}
}

void vCreateNewEntity(taglib *type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment)
{
	basicDisplayLine *newLine = pvPortMalloc(sizeof(basicDisplayLine));
	newLine->type = type;
	newLine->id = id;
	newLine->label = label;
	newLine->value = value;
	newLine->max = max;
	newLine->min = min;
	newLine->increment = increment;

	strcpy(newLine->strValue, strValue);

	if (((taglib*) newLine->type)->strFormatter != NULL)
	{
		((taglib*) newLine->type)->strFormatter(newLine);
	}

	newLine->next = NULL;
	newLine->labelWidget = NULL;
	newLine->valueWidget = NULL;
	vInserIntoList(newLine);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


