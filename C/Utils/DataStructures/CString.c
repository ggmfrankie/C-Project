//
// Created by ertls on 31.07.2026.
//

#include "CString.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "CArrayList.h"
#include "../Logging/Logging.h"
#include "../Makros/Makros.h"
#include "../Makros/Defer.h"

#define IDENTIFIER 0xFADEDBEEF0

static struct _StringHeader_* strAllocate(size_t capacity){
    struct _StringHeader_* header = calloc(1, sizeof(struct _StringHeader_) + capacity + 1);

    if (!header) ERROR_("String allocation failed");

    header->_IDENTIFIER_ = IDENTIFIER;

    return header;
} 

#define strGetHead(s) (&((struct _StringHeader_*)(s))[-1])

#define strAssert(str) assert(str != nullptr); strAssertIsValid(str)

static void strAssertIsValid(char* c){
    if (strGetHead(c)->_IDENTIFIER_ != IDENTIFIER)
        ERROR_("Provided char* is not a Str");
}

static void strResize(Str* s, size_t newCapacity) {
    struct _StringHeader_* header    = strGetHead(*s);
    struct _StringHeader_* newHeader = realloc(header, sizeof(struct _StringHeader_) + newCapacity + 1);
    assert(newHeader != nullptr);

    newHeader->capacity = newCapacity;
    newHeader->size = min(newCapacity, newHeader->size);

    const auto newContent = (Str) (newHeader + 1);
    newContent[newHeader->size] = '\0';
    *s = newContent;
}

Str strNew(size_t capacity) {
    struct _StringHeader_* header = strAllocate(capacity);
    assert(header != nullptr);

    char* data = (void*) (header+1);

    header->capacity =  capacity;
    header->size = 0;
    return data;
}

Str strNew_copy(const char *s) {
    assert(s != nullptr);

    const size_t len = strlen(s);
    struct _StringHeader_* header = strAllocate(len);
    assert(header != nullptr);

    const Str data = (void*) (header+1);
    memcpy(data, s, len);

    header->capacity = header->size = len;
    return data;
}

Str strNew_copyn(const char *s, size_t count) {
    assert(s != nullptr);

    struct _StringHeader_* header = strAllocate(count);
    assert(header != nullptr);

    char* data = (void*) (header+1);
    memcpy(data, s, count);

    header->capacity = header->size = count;
    return data;
}

Str strNew_sprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    va_list argsCopy;
    va_copy(argsCopy, args);

    const size_t len = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);

    if (len < 0) ERROR_("Could not parse formatstring");

    Str out = strNew(len);
    vsnprintf(out, len+1, fmt, argsCopy);
    va_end(argsCopy);

    strGetHead(out)->size = len;
    return out;
}

size_t strLen(Str s) {
    strAssert(s);

    const struct _StringHeader_* header = strGetHead(s);
    return header->size;
}

size_t strCap(Str s) {
    strAssert(s);

    const struct _StringHeader_* header = strGetHead(s);
    return header->capacity;
}

Str strConcat(Str a, Str b) {
    strAssert(a);
    strAssert(b);

    const size_t lenA = strLen(a);
    const size_t lenB = strLen(b);

    struct _StringHeader_* header = strAllocate(lenA + lenB);
    char* data = (void*) (header+1);

    memcpy(data, a, lenA);
    memcpy(data+lenA, b, lenB);
    data[lenA + lenB] = '\0';

    header->capacity = header->size = lenA + lenB;

    return data;
}

Str* cstrSplit(const char* s, char del) {
    assert(s != nullptr);
    const size_t len = strlen(s);
    Str* aOut = nullptr;

    const char* start = s;
    const char* curr = s;

    for (const char* end = curr + len; curr != end; ++curr) {
        const char c = *curr;
        if (c == del) {
            if (curr != start)
                arrPush(aOut, strNew_copyn(start, curr - start));
            start = curr+1;
        }
    }

    if (curr != start) arrPush(aOut, strNew_copyn(start, curr - start));

    return aOut;
}

char strAt(Str s, size_t idx) {
    strAssert(s);
    const size_t len = strLen(s);
    assert(len > idx);
    return s[idx];
}

bool strIsEmpty(Str s) {
    strAssert(s);
    return strLen(s) == 0;
}

bool strIsFull(Str s) {
    strAssert(s);
    return strCap(s) == strLen(s);
}

bool strStartsWith(Str src, Str p) {
    strAssert(src);
    strAssert(p);

    const size_t lenP = strLen(p);
    const size_t lenSrc = strLen(src);

    if (lenSrc < lenP) return false;
    for (int i = 0; i < lenP; ++i) {
        if (src[i] != p[i]) return false;
    }
    return true;
}

void strAppend(Str* s, char c){
    assert(c != '\0');
    strAssert(*s);
    TODO_("KB");
}

void strAppendAt(Str* s, char c, size_t idx) {
    strAssert(*s);
    const size_t len = strLen(*s);
    if (idx > len) ERROR_("Index out of bounds");

    const size_t cap = strCap(*s);
    if (len == cap) strResize(s, cap*2);

    char* slot = *s + idx;

    memmove(slot+1, slot, len - idx);
    *s[idx] = c;
    *s[++strGetHead(*s)->size] = '\0';
}

void strAppend_sprintf_va(Str* s, const char* fmt, va_list args) {
    assert(fmt != nullptr);
    strAssert(*s);

    va_list argsCopy;
    va_copy(argsCopy, args);

    const size_t len = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);
    if (len < 0) ERROR_("Could not parse formatstring");

    const size_t size = strGetHead(*s)->size;
    const size_t capacity = strGetHead(*s)->capacity;

    const size_t required = size + len;

    if (required > capacity) strResize(s, required);

    vsnprintf(*s + strGetHead(*s)->size, len+1, fmt, argsCopy);
    va_end(argsCopy);

    strGetHead(*s)->size = required;
}

char strPopAt(Str* s, size_t idx) {
    strAssert(*s);
    const size_t len = strLen(*s);
    if (idx > len) ERROR_("Index out of bounds");

    const size_t cap = strCap(*s);
    if (len < cap/2) strResize(s, cap/2);

    char* slot = *s + idx;
    const char out = *slot;

    memmove(slot, slot+1, len - idx);
    *s[--strGetHead(*s)->size] = '\0';

    return out;
}

void strAppend_sprintf(Str* s, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    strAppend_sprintf_va(s, fmt, args);
    va_end(args);
}

void strFit(Str* s) {
    strAssert(*s);
    const size_t size = strLen(*s);
    const size_t cap  = strCap(*s);

    if (cap == size) return;
    strResize(s, size);
}

void strClear(Str* s) {
    strAssert(*s);
    (*s)[0] = '\0';
    strGetHead(*s)->size = 0;
}

void strDelete(Str* s) {
    strAssert(*s);
    free(strGetHead(*s));
    *s = nullptr;
}

Str cstrConcat(const char* a, const char* b) {
    assert(a != nullptr && b != nullptr);
    const size_t lenA = strlen(a);
    const size_t lenB = strlen(b);

    struct _StringHeader_* header = strAllocate(lenA + lenB);
    if (header == nullptr) ERROR_("Allocation failed");

    char* data = (void*) (header+1);

    memcpy(data, a, lenA);
    memcpy(data+lenA, b, lenB);
    data[lenA + lenB] = '\0';

    header->capacity = header->size = lenA + lenB;

    return data;
}

void cstrbConcat(char *buff, size_t size, const char *a, const char *b) {
    assert(a != nullptr && b != nullptr);
    const size_t lenA = strlen(a);
    const size_t lenB = strlen(b);
    const size_t total = lenA + lenB;

    if(size < total + 1) {
        ERROR_("Buffer of size: %llu is not sufficient for string length %llu\n", size, total);
        return;
    }
    memcpy(buff, a, lenA);
    memcpy(buff+lenA, b, lenB);
    buff[total] = '\0';
}

#define content(a, b) assert(strcmp(a, b) == 0)
#define length(s, size) assert(strLen(s) == size)
void _strTest() {
    defer(defer_strDelete) Str a = strNew_copy("hassan");
    content(a, "hassan");
    length(a, 6);

    defer(defer_strDelete) Str b = strNew_copyn("belsa kaka", 5);
    content(b, "belsa");

    defer(defer_strDelete) Str ab = strConcat(a, b);
    content(ab, "hassanbelsa");

    for strEach(c, ab) {

    }
}
#undef content
#undef length