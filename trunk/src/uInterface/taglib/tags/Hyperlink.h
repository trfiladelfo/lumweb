/*
 * Hyperlink.h
 *
 *  Created on: Mar 8, 2010
 *      Author: d3f3nd3r
 */

#ifndef HYPERLINK_H_
#define HYPERLINK_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vParseHyperlink(char* param, int len);
void io_get_hyperlink(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* HYPERLINK_H_ */
