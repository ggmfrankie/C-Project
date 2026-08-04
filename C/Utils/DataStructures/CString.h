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
Str strNew_n(const char* s, size_t count);
Str strNew_c(size_t capacity);
Str strFrom_int(int val);

//const
size_t strLen(CStr s);
size_t strCap(CStr s);
char* strConcat(CStr a, CStr b);
CStr* strSplit(CStr s, char del);
char strAt(CStr a, size_t idx);

bool strIsEmpty(CStr s);
bool strIsFull(CStr s);
bool strStartsWith(CStr src, CStr p);

//mut
void strAppend(Str s);
void strFit(Str s);

void strClear(Str s);
void strDelete(Str s);

char* cstrConcat(const char* a, const char* b);

void _strTest();

#ifdef __cplusplus
}
#endif