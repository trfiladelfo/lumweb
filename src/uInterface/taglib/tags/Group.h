/*
 * Group.h
 *
 *  Created on: Mar 8, 2010
 *      Author: d3f3nd3r
 */

#ifndef GROUP_H_
#define GROUP_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vParseGroup(char* param, int len);
void io_get_group(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* GROUP_H_ */
