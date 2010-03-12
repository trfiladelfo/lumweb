/**
 * \addtogroup Tags
 * @{
 *
 * \file SubmitInputField.h
 * \author Anziner, Hahn
 * \brief Prototypes for the SubmitInputField tag
 *
*/

#ifndef SUBMITINPUTFIELD_H_
#define SUBMITINPUTFIELD_H_

#include "taglib/taglib.h"
#include "taglib/tags.h"

#include "ethernet/httpd/cgi/ssiparams.h"

void io_get_submit_input_button(char * pcBuf, int iBufLen, pSSIParam *params);

#endif /* SUBMITINPUTFIELD_H_ */
