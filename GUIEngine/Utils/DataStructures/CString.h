//
// Created by ertls on 18.09.2026.
//

#pragma once
#include <stdint.h>

#include "Utils/DataStructures/CStr.h"

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
String String_newn(const char* cString, size_t len);
void String_clear(String* s);
void String_free(String* s);

const char* String_getCValue(const String* s);
size_t String_getLen(String s);

bool String_equals(String s1, String s2);

void String_test();
