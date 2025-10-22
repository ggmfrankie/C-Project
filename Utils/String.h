#ifndef STRING_H
#define STRING_H

#include <stdbool.h>
#include "ArrayList.h"
#include "GenericLinkedList.h"

typedef struct List_String List_String;

typedef struct String {
    char* content;
    int length;
    char (*charAt)(const struct String* string,int);
    struct String (*substring)(const struct String* string, int start_index, int end_index);
    struct String (*combine)(const struct String *string1, const struct String *string2);
    void (*delete)(const struct String* string);
    void (*clear)(struct String* string);
    void (*println)(const struct String* string);
    bool (*equals)(const struct String* string, const struct String* key);
    bool (*startsWith)(const struct String* string, const struct String* key);
    List_String (*split)(const struct String* string, const char* key);
} String;



ARRAY_LIST(String, String)
DEFINE_LINKED_LIST(String, String)

char str_getCharAt(const String* string, int index);
String str_substring(const String* string, int start_index, int end_index);
String newString(char* content);
String newString_c(const char* content);
String str_combine(const String *string1, const String *string2);
void str_delete(const String* string);
void str_clear(String* string);
void str_println(const String* string);
bool str_equals(const String* string, const String* key);
bool str_startsWith(const String* string, const String* key);
List_String str_split(const String* string, const char* key);


#endif