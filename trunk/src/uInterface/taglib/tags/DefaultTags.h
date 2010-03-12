/**
 * \addtogroup Tags
 * @{
 *
 * \file DefaultTags.h
 * \author Anziner, Hahn
 * \brief Prototypes for dummy default tag
 *
*/

#ifndef DEFAULTTAGS_H_
#define DEFAULTTAGS_H_

void vDummyOnLoadPtr(char* param, int len, void* this);
void vDummyOnEditValuePtr(void* this);
void vDummyOnDestroyPtr(void* this);
char* vDummyStrFormatter (void* this);

#endif /* DEFAULTTAGS_H_ */
