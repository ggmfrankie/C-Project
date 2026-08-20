//
// Created by Stefan on 21.05.2026.
//

#pragma once
#include <stdio.h>

typedef const char* CStr;

void defer_closeFile(FILE** f);
void defer_free(void* p);
void defer_strDelete(CStr* s);
void defer_arrDelete(void* a);

#define defer(func) __attribute__((cleanup(func)))


