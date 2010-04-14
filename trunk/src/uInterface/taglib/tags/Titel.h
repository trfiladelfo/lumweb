/**
 * \addtogroup Tags
 * @{
 *
 * \file Titel.h
 * \author Anziner, Hahn
 * \brief Prototypes for the Titel tag
 *
 */

#ifndef TITEL_H_
#define TITEL_H_

#include "ethernet/httpd/cgi/ssiparams.h"

void vTitleOnLoad(char* param, int len, void* this);
void vTitleRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* TITEL_H_ */
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
