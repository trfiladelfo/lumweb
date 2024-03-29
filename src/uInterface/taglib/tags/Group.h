/**
 * \addtogroup Tags
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for the Group tag
 *
 */

#ifndef GROUP_H_
#define GROUP_H_

#include "ethernet/httpd/cgi/ssiparams.h"

void vGroupOnLoad(char* param, int len, void* this);
void vGroupRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* GROUP_H_ */

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
