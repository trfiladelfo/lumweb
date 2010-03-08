/*
 * Titel.h
 *
 *  Created on: Mar 8, 2010
 *      Author: d3f3nd3r
 */

#ifndef TITEL_H_
#define TITEL_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vParseTitle(char* param, int len);
void io_get_titel(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* TITEL_H_ */
