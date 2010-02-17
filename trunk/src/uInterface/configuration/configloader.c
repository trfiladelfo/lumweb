/**
 * \addtogroup Config
 * @{
 *
 * \file configloader.c
 * \author Anziner, Hahn
 * \brief Routines for conf file handling
 *
*/


#include "lmi_fs.h"
#include "hw_types.h"
#include "setup.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "graphic/graphicLib.h"

#include "FreeRTOS.h"
#include "task.h"

/// Enables debug messages for config handling
#define CONFIG_DEBUG 1

/// length of read buffer
#define READBUFFERLEN 	128

/// pointer to the config file
struct fs_file *config_file;

/// read buffer
char buffer[READBUFFERLEN];


char* paramAndValueFound(char* param, char* value, int paramLen, int valueLen) {
	char* returnValue;
	int j;
	if (paramLen > 0) {
		paramLen = 0;
		if (valueLen > 0) {
			returnValue = (char*) pvPortMalloc(valueLen * sizeof(char) + 1);
			for (j = 0; j < valueLen && value[j] > 0x20 && value[j] < 0x7F; j++) {
				returnValue[j] = value[j];
			}
			returnValue[j] = 0;
			 strtrim(returnValue);
			 xTaskResumeAll();
			return returnValue;
		}
		for (j = 0; j < READBUFFERLEN; j++) {
			param[j] = 0;
		}
	}
	xTaskResumeAll();
	return NULL;
}

/**
 *
 * searchs the config file for a parameter and returns the value
 *
 */
char* loadFromConfig(char* filePath, char* param) {
	int i, j;
	tBoolean beforeEqual = true, isInComment = false;

	vTaskSuspendAll();

	char name[READBUFFERLEN];
	char value[READBUFFERLEN];

	char last = '\n';

	int nameLen = 0;
	int valueLen = 0;
	config_file = fs_open(filePath);

	if (config_file != NULL) {
		while (fs_read(config_file, buffer, READBUFFERLEN) > 0) {

			for (i = 0; i < READBUFFERLEN; i++) {
				if (last == '\n') {
					beforeEqual = true;
					isInComment = false;
					for (j = 0; j < READBUFFERLEN; j++) {
						name[j] = 0;
						value[j] = 0;
					}
					nameLen = 0;
					valueLen = 0;
				}
				if (isInComment == true || buffer[i] == '#') {
					if (buffer[i] == '#' && (nameLen > 0 || valueLen > 0)) {

						if (strcmp(name, param) == 0 && beforeEqual == false) {
							fs_close(config_file);
							return paramAndValueFound(name, value, nameLen,
									valueLen);
						} else {
							for (j = 0; j < READBUFFERLEN; j++) {
								name[j] = 0;
								value[j] = 0;
							}
						}
					}
					isInComment = true;
					while (i < READBUFFERLEN && buffer[i] != '\n') {
						i++;
					}
					last = buffer[i];
				} else {
					//	printf("CONFIG: read char 0x%X (%c)\n", buffer[i],
					//			buffer[i]);

					if (buffer[i] == '=' && nameLen > 0) {
						name[nameLen] = 0;
						beforeEqual = false;

						if (strcmp(name, param) != 0) {
							while (i < READBUFFERLEN && buffer[i] != '\n') {
								i++;
							}
							for (j = 0; j < READBUFFERLEN; j++) {
								name[j] = 0;
							}
						}
					} else if (buffer[i] == '\n') {
						isInComment = false;
						beforeEqual = true;
						if (strcmp(name, param) == 0) {
							fs_close(config_file);
							return paramAndValueFound(name, value, nameLen,
									valueLen);
						} else {
							for (j = 0; j < READBUFFERLEN; j++) {
								name[j] = 0;
								value[j] = 0;
							}
						}
					} else {
						if (beforeEqual) {
							name[nameLen] = buffer[i];
							nameLen++;
						} else {
							value[valueLen] = buffer[i];
							valueLen++;
						}
					}

					last = buffer[i];
				}
			}
		}
	} else {
		#ifdef ENABLE_GRAPHIC
			vShowBootText("Please insert a correct MicroSD card!");
		#endif
		printf("CONF: File can't be opened");
	}
	fs_close(config_file);
	xTaskResumeAll();
	return NULL;
}
