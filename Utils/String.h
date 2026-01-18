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
    size_t capacity;
    bool isHeap;

} String;

typedef struct StringFunctions {
    char (*charAt)(const String* string,int);
    String (*substring)(const String* string, int start_index, int end_index);
    String (*combine)(const String *string1, const String *string2);
    String (*fromInt_c)(int value);
    String (*newReservedString)(size_t capacity);
    void (*fromInt)(char* content, size_t size, long long value);

    void (*delete)(String* string);
    void (*clear)(String* string);
    void (*recalculateLength)(String* string);
    void (*setContent_c)(String* string, const char* content);
    void (*println)(const String* string);
    bool (*equals)(const String* string, const String* key);
    bool (*startsWith)(const String* string, const String* key);
    List_String (*split)(const String* string, const char* key);
    void (*setCharAt)(const String* string, int index, char value);
    void (*setAll)(const String* string, char key);

} StringFunctions;

ARRAY_LIST(String, String)
DEFINE_LINKED_LIST(String, String)


String stringOf(char* content);
String newString_c(const char* content);
String newEmptyString(size_t length);
String str_combine(const String *string1, const String *string2);
String str_substring(const String* string, int start_index, int end_index);
String str_fromInt_c(int value);
String newString_array(char* content, size_t capacity);
String newReservedString(size_t capacity);
void str_fromInt(char* content, size_t size, long long value);

void str_delete(String* string);
void str_clear(String* string);
void str_recalculateLength(String* string);
void str_setContent_c(String* string, const char* content);
void str_println(const String* string);
bool str_equals(const String* string, const String* key);
bool str_startsWith(const String* string, const String* key);
List_String str_split(const String* string, const char* key);
void str_setCharAt(const String* string, int index, char value);
void str_setAll(const String* string, char key);
char str_getCharAt(const String* string, int index);


extern StringFunctions Strings;



#endif