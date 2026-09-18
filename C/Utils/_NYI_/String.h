//
// Created by ertls on 18.09.2026.
//

#pragma once
#include <stdint.h>

#include "Utils/DataStructures/CString.h"

/**
 * @brief <[tag|length][\0][c5][c4][c3][c2][c1][c0]>
 *        <Str>
 */
typedef struct {
    union {
        Str heapString;

        struct {
            char m[7];
            uint8_t len;
        } inlineString;
    };
} String;

String String_new(const char* cString);
void String_clear(String* s);
void String_free(String* s);
char* String_getValue(String s);
size_t String_getLen(String s);

void String_test();
