/**
 * \addtogroup Tags
 * @{
 *
 * \file IntegerInputField.h
 * \author Anziner, Hahn
 * \brief Prototypes for the IntegerInputField tag
 *
 */

#ifndef INTEGERINPUTFIELD_H_
#define INTEGERINPUTFIELD_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void vIntegerOnLoad(char* param, int len, void* this);
char* pcIntegerStrFormatter(void*line);
void vIntegerRenderSSI(char * pcBuf, int iBufLen, pSSIParam *params);
tWidget* xIntegerOnDisplay(void* this, int row);

#endif /* INTEGERINPUTFIELD_H_ */
