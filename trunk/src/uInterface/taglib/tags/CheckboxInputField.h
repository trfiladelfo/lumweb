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


void vCheckboxOnLoad(char* param, int len, void* this);
void vCheckboxRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);
tWidget* xCheckboxOnDisplay(void* this, int row);

#endif /* CHECKBOXINPUTFIELD_H_ */
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
