//
// Created by ertls on 08.09.2026.
//

#pragma once
#include <stddef.h>
#include <stdint.h>

#include "CString.h"

typedef struct {
    union {
        const char* m;
        struct {
            char _unused[7];
            uint8_t len;
        };
    };
} SStrView;

SStrView StrView_new(const char* c);
SStrView StrView_newn(const char* c, uint8_t len);

uint8_t StrView_getLen(SStrView sv);

void StrView_shrinkFront(SStrView* sv, uint8_t len);
void StrView_shrinkBack(SStrView* sv, uint8_t len);

const char* StrView_toTempBuf(SStrView sv);
String StrView_toString(SStrView sv);