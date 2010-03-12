/**
 * \addtogroup Tags
 * @{
 *
 * \file TimeInputField.h
 * \author Anziner, Hahn
 * \brief Prototypes for the TimeInputField tag
 *
*/

#ifndef TIMEINPUTFIELD_H_
#define TIMEINPUTFIELD_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vParseTimeInputField(char* param, int len);
char* pcFormatTimeValue(basicDisplayLine *line);
void io_get_time_input_field(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* TIMEINPUTFIELD_H_ */
