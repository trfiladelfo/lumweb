/*
 * CheckboxInputField.h
 *
 *  Created on: Mar 6, 2010
 *      Author: d3f3nd3r
 */

#ifndef CHECKBOXINPUTFIELD_H_
#define CHECKBOXINPUTFIELD_H_

#include "ethernet/httpd/cgi/ssiparams.h"


void vParseCheckboxInputField(char* param, int len);
void io_get_checkbox_input_field(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* CHECKBOXINPUTFIELD_H_ */
