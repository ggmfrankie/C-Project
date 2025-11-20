#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include "ArrayList.h"
#include "GenericLinkedList.h"

typedef struct List_String List_String;
typedef struct StringFunctions StringFunctions;

typedef struct String {
    char* content;
    size_t length;
    bool isHeap;

} String;

typedef struct StringFunctions {
    char (*charAt)(const struct String* string,int);
    struct String (*substring)(const struct String* string, int start_index, int end_index);
    struct String (*combine)(const struct String *string1, const struct String *string2);
    void (*delete)(struct String* string);
    void (*clear)(struct String* string);
    void (*println)(const struct String* string);
    bool (*equals)(const struct String* string, const struct String* key);
    bool (*startsWith)(const struct String* string, const struct String* key);
    List_String (*split)(const struct String* string, const char* key);
    void (*setCharAt)(const struct String* string, int index, char value);
    void (*setAll)(const struct String* string, char key);
} StringFunctions;

ARRAY_LIST(String, String)
DEFINE_LINKED_LIST(String, String)


String stringOf(char* content);
String newString_c(const char* content);
String newEmptyString(size_t length);
String str_combine(const String *string1, const String *string2);
String str_substring(const String* string, int start_index, int end_index);

void str_delete(String* string);
void str_clear(String* string);
void str_println(const String* string);
bool str_equals(const String* string, const String* key);
bool str_startsWith(const String* string, const String* key);
List_String str_split(const String* string, const char* key);
void str_setCharAt(const String* string, int index, char value);
void str_setAll(const String* string, char key);
char str_getCharAt(const String* string, int index);


static StringFunctions Strings = {
    .charAt = str_getCharAt,
    .combine = str_combine,
    .delete = str_delete,
    .clear = str_clear,
    .equals = str_equals,
    .println = str_println,
    .split = str_split,
    .startsWith = str_startsWith,
    .substring = str_substring,
    .setAll = str_setAll,
    .setCharAt = str_setCharAt
};



#endif