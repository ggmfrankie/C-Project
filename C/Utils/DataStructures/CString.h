//
// Created by ertls on 31.07.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

#define STR_DEBUG 1

typedef char* Str;

struct _StringHeader_ {
    #if STR_DEBUG
    size_t _IDENTIFIER_;
    #endif
    size_t size;
    size_t capacity;
};

//@brief creates an empty Str with supplied capacity
Str strNew(size_t capacity);
Str strNew_copy(const char* s);
//@brief create a new Str from char* until specified length
Str strNew_copyn(const char* s, size_t count);

//@brief creates a new Str from the provided format string
Str strNew_sprintf(const char* fmt, ...);

//@brief gives back the length as stored inside the Str
size_t strLen(Str s);
//@brief gives back the capacity as stored inside the Str
size_t strCap(Str s);
//@brief takes two Str and concatenates then into a new Str
char* strConcat(Str a, Str b);
//@brief takes a Str and splits it into an Array of Str with the given delimiter
Str* cstrSplit(const char* s, char del);
//@brief tries to return the char at the given index
char strAt(Str s, size_t idx);

//@brief returns true if the underlying Str is empty
bool strIsEmpty(Str s);
//@brief returns true if the underlying Str is full
bool strIsFull(Str s);
//@brief returns if de src Str contains p in the beginning
bool strStartsWith(Str src, Str p);

//@brief appends the char to the end of the Str. May reallocate the Str
void strAppend(Str* s, char c);
void strAppendAt(Str* s, char c, size_t idx);

void strAppend_sprintf(Str* s, const char* fmt, ...);
void strAppend_sprintf_va(Str* s, const char* fmt, va_list args);

char strPopAt(Str* s, size_t idx);

void strCopy(Str* dest, const char* source);
//@brief makes the capacity match the size of the Str. May reallocate the Str
void strFit(Str* s);
//@brief sets the content to '\0' and size to 0. Keeps capacity
void strClear(Str* s);
//@brief deletes the underlying memory and sets the Str to nullptr
void strDelete(Str* s);

#define strEach(c, str) (size_t i = 0, _len = strLen(str); i < _len; i++) for (char (c) = str[i]; (c) != '\0'; (c) = '\0')

Str cstrConcat(const char* a, const char* b);
void cstrbConcat(char* buff, size_t size, const char* a, const char* b);

void _strTest();

#ifdef __cplusplus
}
#endif