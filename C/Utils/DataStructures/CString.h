//
// Created by ertls on 31.07.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>

typedef char* Str;
typedef const char* CStr;

struct _StringHeader_ {
    size_t size;
    size_t capacity;
};

//new
Str strNew(const char* s);
//@brief create a new Str from char* until specified length
Str strNew_n(const char* s, size_t count);
//@brief creates an empty Str with supplied capacity
Str strNew_c(size_t capacity);
//@brief creates a new Str from an integer
Str strFrom_int(int val);

//@brief gives back the lenght as stored inside the Str
size_t strLen(CStr s);
//@brief gives back the capacity as stored inside the Str
size_t strCap(CStr s);
//@brief takes two Str and concatinates then into a new Str
char* strConcat(CStr a, CStr b);
//@brief takes a Str and splits it into an Array of Str with the given delimiter
Str* strSplit(CStr s, char del);
//@brief tries to return the char at the given index
char strAt(CStr a, size_t idx);

bool strIsEmpty(CStr s);
bool strIsFull(CStr s);
bool strStartsWith(CStr src, CStr p);

//mut
void strAppend(Str s);
void strFit(Str s);

void strClear(Str s);
void strDelete(Str s);

Str cstrConcat(const char* a, const char* b);
void cstrbConcat(char* buff, size_t size, char* a, const char* b);

void _strTest();

#ifdef __cplusplus
}
#endif