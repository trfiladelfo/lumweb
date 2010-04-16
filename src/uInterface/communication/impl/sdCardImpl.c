/**
 * \addtogroup comTask
 * @{
 *
 * \author Anziner, Hahn
 * \brief implements a simple testroutine to store and read the values from/to the SD-Card
 *
 *
 */

/* std lib includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "../comTask.h"

/* FatFs includes */
#include "fatfs/ff.h"
#include "lmi_fs.h"

#include "setup.h"

#define PATH_TO_DATA	"data/"

char path_buf[32], buf[32];
FIL save_file;

// Empty Init Routine
void vComTaskInitImpl(void)
{
}

int sendToMachine(char* id, int value)
{
	unsigned int bw;
	int rc = 0, error = 0;

	// suspend all other tasks
	vTaskSuspendAll();

	fs_enable(400000);

	strcat(path_buf, PATH_TO_DATA);
	strncat(path_buf, id, 8);

#if DEBUG_COM
	printf("SendToMachine: opening file: %s \n", path_buf);
#endif

	if (strcmp(id, "kurve") == 0)
	{
		if (value < 10 || value > 20)
		{
			error = 1;
		}
	}

	rc = f_open(&save_file, path_buf, FA_CREATE_NEW);

	if (rc == FR_EXIST)
		rc = f_open(&save_file, path_buf, FA_WRITE);

	if (rc == FR_OK)
	{

		sprintf(buf, "%d", value);
		if (error == 0)
		{
			rc = f_write(&save_file, &buf, strlen(buf), &bw);
#if DEBUG_COM
			printf("SendToMachine: rc: %d - wrote '%s' to file\n",rc, buf);
#endif
		}
		f_sync(&save_file);
		f_close(&save_file);

	}
	else
	{
#if DEBUG_COM
		printf("SendToMachine: Error opening file, rc=%d\n", rc);
#endif
	}
	path_buf[0] = 0;
	buf[0] = 0;

	// resumes all tasks
	xTaskResumeAll();

	return rc;
}

int getFormMachine(char* id)
{
	int value = -999; // error code
	int rc;

	// suspend all other tasks
	vTaskSuspendAll();

	fs_enable(400000);

	strcat(path_buf, PATH_TO_DATA);
	strncat(path_buf, id, 8);

#if DEBUG_COM
	printf("getFormMachine: opening file: '%s' \n", path_buf);
#endif

	rc = f_open(&save_file, path_buf, FA_READ);

	if (rc == FR_OK)
	{
		f_gets(buf, 32, &save_file);
		value = atoi(buf);
		f_close(&save_file);
#if DEBUG_COM
		printf("getFormMachine: rc: %d - read '%d' from file\n",rc, value);
#endif
	}
	else
	{
#if DEBUG_COM
		printf("getFormMachine: Error opening file, rc=%d\n", rc);
#endif
	}

	path_buf[0] = 0;
	buf[0] = 0;

	// resumes all tasks
	xTaskResumeAll();

	return value;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

