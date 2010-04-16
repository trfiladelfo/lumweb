/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for the TimeInputField tag
 *
 */

#ifndef TIMEINPUTFIELD_H_
#define TIMEINPUTFIELD_H_

#include "ethernet/httpd/cgi/ssiparams.h"

void vTimeOnLoad(char* param, int len, void* this);
char* pcTimeStrFormatter(void* line);
void vTimeRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);
tWidget* xTimeOnDisplay(void* this, int row);

#endif /* TIMEINPUTFIELD_H_ */
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
