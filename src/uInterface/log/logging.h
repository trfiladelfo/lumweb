/**
 * \addtogroup logging
 * @{
 *
 * \author Anziner, Hahn
 * \brief Prototypes for handling log files
 *
 *
 */

//*****************************************************************************
//
// logging.h - Prototypes for handling log files
//
//*****************************************************************************

#ifndef LOGGING_H_
#define LOGGING_H_

#include <string.h>

#include "fatfs/ff.h"

//*****************************************************************************
//
/// Defines path to the log file
//
//*****************************************************************************
#define LOG_FILE_PATH "log/sys.log"

//*****************************************************************************
//
/// global pointer to the log file
//
//*****************************************************************************
FIL* log_file;

/** Opens the log file (path defined as LOG_FILE_PATH) and
 *  sets file pointer to the end of file
 */
FRESULT initLog();

/**
 * Appends the message to the log file and adds the current time
 *
 */
FRESULT appendToLog(char *msg);

#endif

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

