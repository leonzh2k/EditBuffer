#ifndef EDITBUFFER_H 	 	    		
#define EDITBUFFER_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"{
#endif

#define EDIT_BUFFER_ORIGIN_BEGINNING    0
#define EDIT_BUFFER_ORIGIN_CURRENT      1
#define EDIT_BUFFER_ORIGIN_END          2

typedef struct STRUCT_EDIT_BUFFER_TAG SEditBuffer;
typedef SEditBuffer * SEditBufferRef;

SEditBufferRef EditBufferCreate();

void EditBufferDestroy(SEditBufferRef ebuf);

void PrintEditBuffer(SEditBufferRef ebuf);

size_t EditBufferChars(SEditBufferRef ebuf);

size_t EditBufferSize(SEditBufferRef ebuf);

size_t EditBufferMoveCursor(SEditBufferRef ebuf, int offset, int origin);

size_t EditBufferLoad(SEditBufferRef ebuf, const char *buf);

size_t EditBufferRead(SEditBufferRef ebuf, char *buf, size_t count);

size_t EditBufferInsert(SEditBufferRef ebuf, const char *buf, size_t count);

size_t EditBufferDelete(SEditBufferRef ebuf, size_t count);

void printOptions();

void loadStringIntoBuffer(SEditBufferRef EditBuffer);
 
void moveBufferCursor(SEditBufferRef EditBuffer);

void insertCharsIntoBuffer(SEditBufferRef EditBuffer);

void deleteCharsFromBuffer(SEditBufferRef EditBuffer);

void printCharsInBuffer(SEditBufferRef EditBuffer);

void printBufferSize(SEditBufferRef EditBuffer);

void newLine();

#ifdef __cplusplus
}
#endif

#endif
