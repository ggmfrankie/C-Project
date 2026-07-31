//
// Created by ertls on 31.07.2026.
//

#include "CString.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../Makros.h"

static struct _StringHeader_* _strGetHeader(CStr s) {
    return &((struct _StringHeader_*)(s))[-1];
}

static void _strResizeTo(Str s, size_t newCap) {
    struct _StringHeader_* header = _strGetHeader(s);
    struct _StringHeader_* newHeader = realloc(header, sizeof(struct _StringHeader_) + newCap);
    assert(newHeader != nullptr);
    newHeader->capacity = newCap;
    newHeader->size = min(newCap, newHeader->size);
    header = newHeader;
}

Str strNew(const char *s) {
    assert(s != nullptr);

    const size_t len = strlen(s);
    struct _StringHeader_* header = malloc(sizeof(struct _StringHeader_) + len + 1);
    assert(header != nullptr);

    Str data = (void*) (header+1);
    strcpy(data, s);

    header->capacity = header->size = len;
    return data;
}

Str strNew_n(const char *s, size_t count) {
    assert(s != nullptr);

    const size_t len = count;
    struct _StringHeader_* header = malloc(sizeof(struct _StringHeader_) + len + 1);
    assert(header != nullptr);

    char* data = (void*) (header+1);
    const errno_t result = strncpy_s(data, len + 1, s, len);
    assert(result == 0);

    header->capacity = header->size = len;
    return data;
}

Str strNew_c(size_t capacity) {
    struct _StringHeader_* header = malloc(sizeof(struct _StringHeader_) + capacity + 1);
    assert(header != nullptr);

    char* data = (void*) (header+1);

    header->capacity =  capacity;
    header->size = 0;
    return data;
}

Str strFrom_int(int val) {
    int i = 0;

    const int sign = val;

    if (val < 0) val = -val;
}

size_t strLen(CStr s) {
    if (s == nullptr) return 0;
    const auto header = _strGetHeader(s);
    return header->size;
}

size_t strCap(CStr s) {
    if (s == nullptr) return 0;
    const auto header = _strGetHeader(s);
    return header->capacity;
}

char* strConcat(CStr a, CStr b) {
    assert(a != nullptr && b != nullptr);
    const size_t lenA = strLen(a);
    const size_t lenB = strLen(b);

    struct _StringHeader_* header = malloc(sizeof(struct _StringHeader_) + lenA + lenB + 1);
    char* data = (void*) (header+1);

    memcpy(data, a, lenA);
    memcpy(data+lenA, b, lenB);
    data[lenA + lenB] = '\0';

    header->capacity = header->size = lenA + lenB;

    return data;
}

CStr* strSplit(CStr s, char del) {
}

char strAt(CStr a, size_t idx) {
    const size_t len = strLen(a);
    assert(len > idx);
    return a[idx];
}

bool strIsEmpty(CStr s) {
    return strLen(s) == 0;
}

bool strIsFull(CStr s) {
    return strCap(s) == strLen(s);
}

bool strStartsWith(CStr src, CStr p) {
    const size_t lenP = strLen(p);
    if (strLen(src) < lenP) return false;
    for (int i = 0; i < lenP; ++i) {
        if (src[i] != p[i]) return false;
    }
    return true;
}

void strAppend(Str s);

void strFit(Str s) {
    const size_t size = strLen(s);
    const size_t cap = strCap(s);

    if (cap == size) return;
    _strResizeTo(s, size);

}

void strClear(Str s) {
    memset(s, 0, strLen(s));
    _strGetHeader(s)->size = 0;
}
void strDelete(Str s) {
    free(_strGetHeader(s));
}

#define content(a, b) assert(strcmp(a, b) == 0)
#define length(s, size) assert(strLen(s) == size)
void _strTest() {
    const char* a = strNew("hassan");
    content(a, "hassan");
    length(a, 6);

    const char* b = strNew_n("belsa kaka", 5);
    content(b, "belsa");

    const char* ab = strConcat(a, b);
    content(ab, "hassanbelsa");
}
#undef content
#undef length