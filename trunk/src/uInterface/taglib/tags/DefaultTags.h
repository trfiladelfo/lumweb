/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for dummy default tag
 *
 */

#ifndef DEFAULTTAGS_H_
#define DEFAULTTAGS_H_

#include "ethernet/httpd/cgi/ssiparams.h"

void vDummyOnLoad(char* param, int len, void* this);
tWidget* xDummyOnDisplay(void* this, int row);
void vDummyOnEditValue(void* this);
void vDummyOnDestroy(void* this);
char* pcDummyStrFormatter(void* this);
void vDummyRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* DEFAULTTAGS_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
