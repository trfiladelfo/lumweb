/**
 * \addtogroup Tags
 * @{
 *
 * \file CheckboxInputField.h
 * \author Anziner, Hahn
 * \brief Prototypes for the CheckboxInputField tag
 *
*/

#ifndef CHECKBOXINPUTFIELD_H_
#define CHECKBOXINPUTFIELD_H_

#include "ethernet/httpd/cgi/ssiparams.h"


void vParseCheckboxInputField(char* param, int len, void* this);
void io_get_checkbox_input_field(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* CHECKBOXINPUTFIELD_H_ */
