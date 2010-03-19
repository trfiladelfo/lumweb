/**
 * \addtogroup Tags
 * @{
 *
 * \file SavedParams.h
 * \author Anziner, Hahn
 * \brief Prototypes for the SavedParams tag
 *
*/

#ifndef SAVEDPARAMS_H_
#define SAVEDPARAMS_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void io_print_saved_params(char * pcBuf, int iBufLen, SSIParam *param);

#endif /* SAVEDPARAMS_H_ */
