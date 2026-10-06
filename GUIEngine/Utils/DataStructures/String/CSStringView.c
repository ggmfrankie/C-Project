//
// Created by ertls on 08.09.2026.
//

#include "CSStringView.h"

#include <assert.h>
#include <string.h>

#include "Utils/Macros/Utils.h"

#define data(sv) (char*)((uint64_t)(sv).m & 0x00'ff'ff'ff'ff'ff'ff'ff)

SStrView StrView_new(const char* c) {
    const size_t len = strlen(c);
    assert(len < UINT8_MAX);
    return StrView_newn(c, len);
}

SStrView StrView_newn(const char* c, uint8_t len) {
    SStrView out;
    out.m = c;
    out.len = len;
    return out;
}

uint8_t StrView_getLen(SStrView sv) {
    return sv.len;
}

void StrView_shrinkFront(SStrView *sv, uint8_t len) {
    len = min(len, sv->len);

    sv->m += len;
    sv->len -= len;
}

void StrView_shrinkBack(SStrView *sv, uint8_t len) {
    len = min(len, sv->len);

    sv->len -= len;
}

const char* StrView_toTempBuf(SStrView sv) {
    static char buff[UINT8_MAX];
    memcpy(buff, data(sv), sv.len);
    buff[sv.len] = '\0';
    return buff;
}

String StrView_toString(SStrView sv) {
    return String_newn(data(sv), sv.len);
}
