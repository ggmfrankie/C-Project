//
// Created by ertls on 31.07.2026.
//

#include "CString.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../Logging/Logging.h"
#include "../Makros/Makros.h"
#include "../Makros/Defer.h"

#define IDENTIFIER 0xFADEDBEEF0

static struct _StringHeader_* _strAllocate(size_t capacity){
    struct _StringHeader_* header = malloc(sizeof(struct _StringHeader_) + capacity + 1);
    #if STR_DEBUG 
    header->_IDENTIFIER_ = IDENTIFIER;
    #endif
} 

static struct _StringHeader_* _strGetHeader(CStr s) {
    return &((struct _StringHeader_*)(s))[-1];
}

static bool _strIsStr(const char* c){
    return _strGetHeader(c)->_IDENTIFIER_ == IDENTIFIER; 
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
    memcpy(data, s, len+1);

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
    if (!_strIsStr(s)) ERROR_("Provided char* is not a Str");
    if (s == nullptr) return 0;
    const struct _StringHeader_* header = _strGetHeader(s);
    return header->size;
}

size_t strCap(CStr s) {
    if (!_strIsStr(s)) ERROR_("Provided char* is not a Str");
    if (s == nullptr) return 0;
    const struct _StringHeader_* header = _strGetHeader(s);
    return header->capacity;
}

Str strConcat(CStr a, CStr b) {
    if (!_strIsStr(a) || _strIsStr(b)) ERROR_("Provided char* is not a Str");
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

Str* strSplit(CStr s, char del) {
    TODO_("Not Implemented");
}

char strAt(CStr s, size_t idx) {
    if (!_strIsStr(s)) ERROR_("Provided char* is not a Str");
    const size_t len = strLen(s);
    assert(len > idx);
    return s[idx];
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

void strAppend(Str s, char c){
    TODO_("KB");
}

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
    s = nullptr;
}

char* cstrConcat(const char* a, const char* b) {
    assert(a != nullptr && b != nullptr);
    const size_t lenA = strlen(a);
    const size_t lenB = strlen(b);

    struct _StringHeader_* header = malloc(sizeof(struct _StringHeader_) + lenA + lenB + 1);
    if (header == nullptr) ERROR_("Allocation failed");

    char* data = (void*) (header+1);

    memcpy(data, a, lenA);
    memcpy(data+lenA, b, lenB);
    data[lenA + lenB] = '\0';

    header->capacity = header->size = lenA + lenB;

    return data;
}

void cstrbConcat(char *buff, size_t size, char *a, const char *b) {
    assert(a != nullptr && b != nullptr);
    const size_t lenA = strlen(a);
    const size_t lenB = strlen(b);
    const size_t total = lenA + lenB;

    if(size < total + 1) {
        WARNING_("Buffer of size: %llu is not sufficient for string length %llu\n", size, total);
        return;
    }
    memcpy(buff, a, lenA);
    memcpy(buff+lenA, b, lenB);
    buff[total] = '\0';
}

#define content(a, b) assert(strcmp(a, b) == 0)
#define length(s, size) assert(strLen(s) == size)
void _strTest() {
    defer(defer_strDelete) const char* a = strNew("hassan");
    content(a, "hassan");
    length(a, 6);

    defer(defer_strDelete) const char* b = strNew_n("belsa kaka", 5);
    content(b, "belsa");

    defer(defer_strDelete) const char* ab = strConcat(a, b);
    content(ab, "hassanbelsa");
}
#undef content
#undef length