/*
 * tags.h
 *
 *  Created on: 19.02.2010
 *      Author: root
 */

#ifndef TAGS_H_
#define TAGS_H_
void vParseHyperlink(char* param, int len);
void vParseTitle(char* param, int len);
void vParseGroup(char* param, int len);
void vParseIntegerInputField(char* param, int len);
void vParseFloatInputField(char* param, int len);
void vParseTimeInputField(char* param, int len);
void vParseCheckboxInputField(char* param, int len);
#endif /* TAGS_H_ */
