//
// Created by Stefan on 21.05.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

typedef char* Str;

void defer_closeFile(FILE** f);
void defer_free(void* p);
void defer_arrDelete(void* a);
void defer_CHashTable_free(void* t);

#define defer(func) __attribute__((cleanup(func)))

#ifdef __cplusplus
}
#endif
