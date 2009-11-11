/*
 * configreader.c
 *
 *  Created on: 2.1.2009
 *      Author: root
 */
#include <stdio.h>
#include <string.h>

#include "fs.h"


#include "configReader.h"


#define HTTPD_FS "httpd-fs"

int readConfigFile(char *fileName){
	FILE_HANDLE mainfile;

	FSInit();
	mainfile = FSOpen("main.txt\0");
	if (mainfile != NULL) {
	//	FSRead(mainfile, msg, 30);
		;

	} else {
		;
	}
}

char* getHtml(char *url){
	FILE_HANDLE htmlFile;
	char *path;

	path = (char* ) pvPortMalloc(strlen(url)+strlen(HTTPD_FS)+1);

	strcat(path, HTTPD_FS);
	strcat(path, url);

	FSInit();
	if (FSCreate("testFile") == NULL)
		return "fehler mit fatfs";

	htmlFile = FSOpen(path);
	if (htmlFile != NULL){
		return "WUHUHU";
	}else
		return path;

//	vPortFree(path);
}

