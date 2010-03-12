/**
 * \addtogroup Tags
 * @{
 *
 * \file IntegerInputField.h
 * \author Anziner, Hahn
 * \brief Prototypes for the IntegerInputField tag
 *
*/


#ifndef INTEGERINPUTFIELD_H_
#define INTEGERINPUTFIELD_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"


void vParseIntegerInputField(char* param, int len);
char* pcFormatIntegerValue(basicDisplayLine *line);
void io_get_integer_input_field(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* INTEGERINPUTFIELD_H_ */
