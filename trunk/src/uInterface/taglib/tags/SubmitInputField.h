/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for the SubmitInputField tag
 *
 */

#ifndef SUBMITINPUTFIELD_H_
#define SUBMITINPUTFIELD_H_

#include "ethernet/httpd/cgi/ssiparams.h"

void vSubmitButtonRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);
void vSubmitButtonOnLoad(char* param, int len, void* this);

#endif /* SUBMITINPUTFIELD_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
