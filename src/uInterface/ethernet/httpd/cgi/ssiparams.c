/**
 * \addtogroup CGIandSSI
 * @{
 *
 * \author Anziner, Hahn
 * \brief
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"

#include "ethernet/httpd/cgi/ssiparams.h"

/**
 adds a new element to the list

 @param *root 		pointer to root element of list
 @param *nameValue	pointer to name-value string ($name=$value)

 @return 0	element not added
 */
int SSIParamAdd(pSSIParam *root, char *nameValue)
{
	char *value;
	pSSIParam nParam, tmp = *(root);

#if DEBUG_SSI_PARAMS
	printf("SSIParamAdd: %s \n", nameValue);
#endif

	value = strstr(nameValue, "=");
	value++;

	nParam = pvPortMalloc(sizeof(SSIParam));
	nParam->name = pvPortMalloc(strlen(nameValue) - strlen(value) + 1);
	nParam->value = pvPortMalloc(strlen(value) + 1);

	if (nParam != NULL && nParam->name != NULL && nParam->value != NULL)
	{

		snprintf(nParam->name, (strlen(nameValue) - strlen(value)), "%s",
				nameValue);
		sprintf(nParam->value, "%s", value);

		nParam->name = strtrim(nParam->name);
		nParam->value = strtrim(nParam->value);

#if DEBUG_SSI
		printf("Werte getrimmt\n");
#endif

		if (strlen(nParam->name) > 0)
		{

			nParam->next = tmp;
			*(root) = nParam;
#if DEBUG_SSI_PARAMS
			printf("SSIParamAdd: added element name: '%s' value: '%s' \n",
					nParam->name, nParam->value);
#endif
		}
		else
		{

#if DEBUG_SSI_PARAMS
			printf("SSIParamAdd: didnt insert element, name empty\n");
#endif
		}

	}
	else
	{
#if DEBUG_SSI_PARAMS
		printf(" ... fail\n");
#endif
		if (nParam->name != NULL)
		{
			vPortFree(nParam->name);
		}
		if (nParam->value != NULL)
		{
			vPortFree(nParam->value);
		}
		if (nParam != NULL)
		{
			vPortFree(nParam);
		}
	}
	return 0;
}
/**
 gets an element with $name from the list

 @param  root	root element of list
 @param  *name	pointer to name of the element

 @return element with $name .... element found
 @return NULL .... element not found
 */
pSSIParam SSIParamGet(pSSIParam root, char *name)
{
	pSSIParam ret = NULL;

	while (root != NULL)
	{
#if DEBUG_SSI_PARAMS
		printf("SSIParamGet: element name: '%s' \n", root->name);
#endif
		if (strcmp(root->name, name) == 0)
			return root;
		root = root->next;
	}

	return ret;
}
/**
 gets a value of an element with $name from the list

 @param root	root element of list
 @param *name	pointer to name of the element

 @return string with the value .... element found
 @return NULL .... element not found
 */
char* SSIParamGetValue(pSSIParam root, char *name)
{
	pSSIParam p;
	char* value = NULL;

	p = SSIParamGet(root, name);
	if (p != NULL)
	{
		value = p->value;
#if DEBUG_SSI_PARAMS
		printf("SSIParamGetValue: found value '%s' for %s \n", value, name);
#endif
	}

	return value;
}
/**
 * deletes and frees all elements ot the list
 *
 * @param *root	pointer to root element of list
 *
 */
void SSIParamDeleteAll(pSSIParam *root)
{
	pSSIParam p = (*root), del = NULL;

	while (p != NULL)
	{
#if DEBUG_SSI_PARAMS
		printf("SSIParamDeleteAll: delete element : %s \n", p->name);
#endif
		del = p;
		p = p->next;
		vPortFree(del->name);

#if DEBUG_SSI_PARAMS
		printf("SSIParamDeleteAll: freed name \n");
#endif

		vPortFree(del->value);

#if DEBUG_SSI_PARAMS
		printf("SSIParamDeleteAll: freed value \n");
#endif

		vPortFree(del);
	}

#if DEBUG_SSI_PARAMS
	printf("SSIParamDeleteAll: deleted all elements \n");
#endif
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
