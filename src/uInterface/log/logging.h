
#ifndef LOGGING_H_
#define LOGGING_H_

#include <string.h>

#include "fatfs/ff.h"

#define DEBUG_LOG 	1
#define LOG_FILE_PATH "log/sys.log"

FIL* log_file;

FRESULT initLog();
FRESULT appendToLog(char *msg);

#endif
