//
// Created by ertls on 18.09.2026.
//

#include "CString.h"

#include <stdio.h>
#include <string.h>

static bool String_isInline(String s) {
    return s.inlineString.len != 0;
}

String String_new(const char* cString) {
    const size_t len = strlen(cString);
    return String_newn(cString, len);
}

String String_newn(const char *cString, size_t len) {
    String out = {};

    if (len < 7) {
        memcpy(out.inlineString.m, cString, len);
        out.inlineString.m[len] = '\0';
        out.inlineString.len = len;
    }
    else out.heapString = strNew_copy(cString);

    // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
    return out;
}

void String_clear(String* s) {
    if (String_isInline(*s)) memset(s, 0, sizeof(*s));
    else strClear(s->heapString);
}

void String_free(String* s) {
    if (String_isInline(*s)) memset(s, 0, sizeof(*s));
    else strFree(&s->heapString);
}

const char* String_getCValue(const String* s) {
    return String_isInline(*s) ? s->inlineString.m : s->heapString;
}

size_t String_getLen(String s) {
    return String_isInline(s) ? s.inlineString.len : strLen(s.heapString);
}

bool String_equals(String s1, String s2) {
    const size_t len1 = String_getLen(s1);
    const size_t len2 = String_getLen(s2);
    if (len1 != len2) return false;

    const char* o = String_getCValue(&s1);
    const char* j = String_getCValue(&s2);

    volatile bool same = memcmp(o, j, len1) == 0;

    return same;
}

void String_test() {
    const String a = String_new("Kaka");
    const char* value = String_getCValue(&a);

    printf("%s", value);
}
