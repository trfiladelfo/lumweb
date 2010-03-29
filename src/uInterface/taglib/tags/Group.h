/**
 * \addtogroup Tags
 * @{
 *
 * \file Group.h
 * \author Anziner, Hahn
 * \brief Prototypes for the Group tag
 *
*/

#ifndef GROUP_H_
#define GROUP_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vParseGroup(char* param, int len, void* this);
void io_get_group(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* GROUP_H_ */
