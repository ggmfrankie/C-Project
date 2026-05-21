//
// Created by Stefan on 21.05.2026.
//

#pragma once
#include <stdio.h>

void defer_closeFile(FILE** f);
void defer_free(void* p);

#define defer(func) __attribute__((cleanup(func)))

float deg2rad(float d);
