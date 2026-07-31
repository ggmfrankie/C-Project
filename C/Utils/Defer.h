//
// Created by Stefan on 21.05.2026.
//

#pragma once
#include <stdio.h>

typedef char* Str;

void defer_closeFile(FILE** f);
void defer_free(void* p);
void defer_strDelete(Str* s);

#define defer(func) __attribute__((cleanup(func)))


