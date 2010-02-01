

#include "lmi_fs.h"
#include "hw_types.h"

#include <string.h>

#include "realtime.h"
#include "log/logging.h"


FRESULT initLog(){
	FRESULT rc = FR_NO_FILE;
	log_file = (FIL*) pvPortMalloc(sizeof(FIL));
	fs_enable(400000);

	if(log_file != NULL){
#ifdef DEBUG_LOG
		UARTprintf("Opening file, memory OK \n");
#endif
		rc = f_open(log_file, LOG_FILE_PATH, FA_WRITE);

		//goto end of file
		if(rc == FR_OK)
			f_lseek(log_file, log_file->fsize);
	}

	return rc;
}

FRESULT appendToLog(char *msg){
	FRESULT rc = FR_NO_FILE;
	unsigned int bw, i = 0;
	char buf[128], time_buf[30];


	get_dateandtime(time_buf, sizeof(time_buf));
	i = strlen(time_buf);
	i--;
	time_buf[i] = 0;

	if(log_file != NULL){
		snprintf(&buf, 127, "%s : %s\n", time_buf , msg);
		rc = f_write(log_file, buf, strlen(buf), &bw);

#ifdef DEBUG_LOG
		if(rc == FR_OK)
			UARTprintf("appendToLog: wrote msg %s", buf);
#endif
		f_sync(log_file);
	}
	return rc;
}
