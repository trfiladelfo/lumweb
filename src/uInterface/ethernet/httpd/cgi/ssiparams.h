/*
 * ssiparams.c
 *
 *  Created on: Mar 6, 2010
 *      Author: d3f3nd3r
 */

#ifndef __SSIPARAMS_H__
#define __SSIPARAMS_H__

/** represents an SSI Parameter */
typedef struct SSIParam {
	char *name;
	char *value;
	struct SSIParam* next;
} SSIParam;

typedef SSIParam * pSSIParam;


///  gets an element with $name from the list
int SSIParamAdd(pSSIParam* root, char* nameValue);

///  gets an element with $name from the list
pSSIParam SSIParamGet(pSSIParam root, char* name);

/// deletes and frees all elements ot the list
void SSIParamDeleteAll(pSSIParam* root);

/// gets a value of an element with $name from the list
char* SSIParamGetValue(pSSIParam root, char* name);


#endif
