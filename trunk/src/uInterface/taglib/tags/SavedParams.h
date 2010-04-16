/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for the SavedParams tag
 *
 */

#ifndef SAVEDPARAMS_H_
#define SAVEDPARAMS_H_

#include "ethernet/httpd/cgi/ssiparams.h"

void vSavedParamsRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* SAVEDPARAMS_H_ */
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
