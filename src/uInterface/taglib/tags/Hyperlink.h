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

void vHyperlinkOnLoad(char* param, int len, void* this);
void vHyperlinkRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);
char* pcHyperlinkStrFormatter (void* this);
tWidget* xHyperlinkOnDisplay (void* this, int row);

#endif /* HYPERLINK_H_ */
