/*
 * configloader.c
 *
 *  Created on: 13.01.2010
 *      Author: root
 */

#include "lmi_fs.h"
#include "hw_types.h"
#include <string.h>

#define CONFIG_DEBUG 1

#define READBUFFERLEN 	128

struct fs_file *file;
char buffer[READBUFFERLEN];

char* paramAndValueFound(char* param, char* value, int paramLen, int valueLen) {
	char* returnValue;
	int j;
	if (paramLen > 0) {
		paramLen = 0;
		if (valueLen > 0) {
			printf("CONFIG: name - value pair found: %s == %s\n", param, value);
			returnValue = (char*) pvPortMalloc(valueLen * sizeof(char) + 1);
			for (j = 0; j < valueLen && value[j] > 0x20 && value[j] < 0x7F; j++) {
				returnValue[j] = value[j];
			}
			returnValue[j] = 0;
			 strtrim(returnValue);
			return returnValue;
		}
		for (j = 0; j < READBUFFERLEN; j++) {
			param[j] = 0;
		}
	}
	return NULL;
}

char* loadFromConfig(char* filePath, char* param) {
	int i, j;
	tBoolean beforeEqual, isInComment = false;

	char name[READBUFFERLEN];
	char value[READBUFFERLEN];

	char last = '\n';

	int nameLen = 0;
	int valueLen = 0;
	printf("CONFIG: open file %s\n", filePath);
	file = fs_open(filePath);

	if (file != NULL) {
		printf("CONFIG: file opened\n");
		while (fs_read(file, buffer, READBUFFERLEN) > 0) {

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
							fs_close(file);
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
							fs_close(file);
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
		printf("CONF: File can't be opened");
	}
	fs_close(file);
	return NULL;
}
