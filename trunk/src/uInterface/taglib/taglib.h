/**
 * \addtogroup TagLib
 * @{
 *
 * \author Anziner, Hahn
 * \brief
 *
 */

#ifndef __TAGLIB_H
#define __TAGLIB_H

#include "FreeRTOS.h"
#include "hw_types.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "ethernet/httpd/cgi/ssiparams.h"

/**
 * Structure for the TAG-Definition
 */
typedef struct __taglibStruct
{
	int tagindex;
	char* tagname;

	void (* renderSSI)(char * pcBuf, int iBufLen, pSSIParam *params);

	void (* onLoad)(char*, int, void* basicDisplayLine);
	tWidget* (* onDisplay)(void* basicDisplayLine, int);
	void (* onEditValue)(void* basicDisplayLine);
	void (* onDestroy)(void* basicDisplayLine);
	char* (* strFormatter)(void* basicDisplayLine);

	void* userSpace;
} taglib;

/**
 * Parse a Value String out of the SSI Param
 *
 * @param str           The Complete SSI String
 * @param search        Name from the searched Value
 */

char* pcGetParamFromString(char* str, char* search);

/**
 * Create new Entity on the Display.
 *
 * @param type          Pointer to the Taglib-Structure
 * @param id            Com-Task ID of the Element
 * @param label         Display Label for the Menu
 * @param strValue      Value as String, NULL if not available
 * @param value         Integer Value
 * @param max           Maximum Value
 * @param min           Minimum Value
 * @param increment     Increment Value
 */
void vCreateNewEntity(taglib *type, char* id, char* label, char* strValue,
		int value, int max, int min, int increment);
/**
 * Insert a DisplayLine into the List for Building the GUI
 *
 * @param basicDisplayLine Structure with all Display Information
 */
void vInserIntoList(void* basicDisplayLine);

#endif // End of Header

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
