//
// Created by ertls on 31.07.2026.
//

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include "Utils/Macros/Macros.h"

#define STR_DEBUG 1

typedef char* Str;

struct _StringHeader_ {
    #if STR_DEBUG
    size_t _IDENTIFIER_;
    #endif
    size_t size;
    size_t capacity;
};

/**
 * @brief Creates a new empty Str with the given capacity
 * @param capacity - the Str capacity
 */
Str strNew(size_t capacity);

/**
 * @brief Creates a new Str from a char*
 * @param s - the char* being copied from
 */
Str strNew_copy(const char* s);
/**
 * @brief Creates a new Str from a char* with at most length 'count'
 *        resulting Str is always null terminated
 * @param s - the char* being copied from
 * @param count - the maximal length of the resulting Str
 */
//@brief create a new Str from char* until specified length
Str strNew_copyn(const char* s, size_t count);

//@brief creates a new Str from the provided format string
Str strNew_sprintf(const char* fmt, ...);

/**
 * @brief Gives back the length of the Str
 * @param s - the Str
 */
size_t strLen(Str s);
//@brief gives back the capacity as stored inside the Str
size_t strCap(Str s);
//@brief takes two Str and concatenates then into a new Str
char* strConcat(Str a, Str b);
//@brief takes a Str and splits it into an Array of Str with the given delimiter
Str* strSplit(Str s, char del);
//@brief tries to return the char at the given index
char strAt(Str s, size_t idx);

//@brief returns true if the underlying Str is empty
bool strIsEmpty(Str s);
//@brief returns true if the underlying Str is full
bool strIsFull(Str s);
//@brief returns if de src Str contains p in the beginning
bool strStartsWith(Str src, Str p);

//@brief appends the char to the end of the Str. May reallocate the Str
void strAppend(Str* s, char c);
void strAppendAt(Str* s, char c, size_t idx);

void strAppend_sprintf(Str* s, const char* fmt, ...);
void strAppend_sprintfVa(Str* s, const char* fmt, va_list args);

char strPop(Str* s);

char strPopAt(Str* s, size_t idx);

void strCopy(Str* dest, const char* source);
//@brief makes the capacity match the size of the Str. May reallocate the Str
void strFit(Str* s);
//@brief sets the content to '\0' and size to 0. Keeps capacity
void strClear(Str s);
//@brief deletes the underlying memory and sets the Str to nullptr
void strFree(Str* s);

const char* strEnd(Str s);

#define strEach_impl(_i, _len, c, str) (size_t _i = 0, _len = strLen(str); _i < _len; _i++) for (char (c) = str[_i]; (c) != '\0'; (c) = '\0')
#define strEach(c, str) strEach_impl(CONCAT(_i, __COUNTER__), CONCAT(_len, __COUNTER__), c, str)

#define cstrEach(c, str) (size_t CONCAT(_i, __LINE__) = 0, CONCAT(_len, __LINE__) = strlen(str); CONCAT(_i, __LINE__) < CONCAT(_len, __LINE__); CONCAT(_i, __LINE__)++) for (char (c) = str[CONCAT(_i, __LINE__)]; (c) != '\0'; (c) = '\0')

Str* cstrSplit(const char* s, char del);
Str cstrConcat(const char* a, const char* b);
void cstrbConcat(char* buff, size_t size, const char* a, const char* b);

#ifdef __cplusplus
}
#endif