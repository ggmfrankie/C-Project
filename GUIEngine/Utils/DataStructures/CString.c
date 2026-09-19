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
    String out = {};
    const size_t len = strlen(cString);

    if (len < 7) {
        memcpy(out.inlineString.m, cString, len+1);
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

char* String_getValue(String s) {
    return String_isInline(s) ? s.inlineString.m : s.heapString;
}

size_t String_getLen(String s) {
    return String_isInline(s) ? s.inlineString.len : strLen(s.heapString);
}

void String_test() {
    const String a = String_new("Kaka");
    const char* value = String_getValue(a);

    String_getValue(a)[2] = 'b';

    printf("%s", value);
}
