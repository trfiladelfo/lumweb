/**
 * \addtogroup Tags
 * @{
 *
 * \file Hyperlink.h
 * \author Anziner, Hahn
 * \brief Prototypes for the Hyperlink tag
 *
*/

#ifndef HYPERLINK_H_
#define HYPERLINK_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vParseHyperlink(char* param, int len, void* this);
void io_get_hyperlink(char * pcBuf, int iBufLen, pSSIParam *params);
char* vHyperlinkStrFormatter (void* this);

#endif /* HYPERLINK_H_ */
