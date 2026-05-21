//
// Created by Stefan on 21.05.2026.
//

#include "Utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void defer_closeFile(FILE** f) {
    if (*f) fclose(*f);
}

void defer_free(void* p) {
    free(*(void**)p);
}

float deg2rad(const float d) { return d * (M_PI / 180.0f); }