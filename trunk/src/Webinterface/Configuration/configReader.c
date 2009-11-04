/*
 * configreader.c
 *
 *  Created on: 2.1.2009
 *      Author: root
 */
#include <stdio.h>

#include "fs.h"


#include "configReader.h"


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
