#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include "ArrayList.h"
#include "GenericLinkedList.h"


typedef struct String {
    char* content;
    int length;

} String;

ARRAY_LIST(String, String)
DEFINE_LINKED_LIST(String, String)

char str_getCharAt(const String* string, int index);
String str_substring(const String* string, int start_index, int end_index);
String str_newString(char* content);
String str_newString_c(const char* content);
String str_combine(const String *string1, const String *string2);
void str_delete(const String* string);
void str_println(const String* string);
bool str_equals(const String* string, const String* key);
bool str_startsWith(const String* string, const String* key);
List_String str_split(const String* string, const String* key);

#endif