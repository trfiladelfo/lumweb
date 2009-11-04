/*
 * configreader.c
 *
 *  Created on: 2.1.2009
 *      Author: root
 */


#include "fs.h"


#include "configreader.h"


int readConfigFile(char *fileName){
	FILE_HANDLE mainfile;
	char *msg;

	msg = pvPortMalloc(30);

	FSInit();
	mainfile = FSOpen("main.txt\0");
	if (mainfile != NULL) {
		FSRead(mainfile, msg, 30);

		xGraph_msg.msg = msg;

	} else {
		xGraph_msg.msg = "READ ERROR!";
	}
}
