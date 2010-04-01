/**
 * \addtogroup Tags
 * @{
 *
 * \file FloatInputField.h
 * \author Anziner, Hahn
 * \brief Protorypes for the FloatInputField tag
 *
*/
#ifndef FLOATINPUTFIELD_H_
#define FLOATINPUTFIELD_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vFloatOnLoad(char* param, int len, void* this);
char* pcFloatStrFormatter(void *line);
void vFloatRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);
tWidget* xFloatOnDisplay(void* this, int row);

#endif /* FLOATINPUTFIELD_H_ */
