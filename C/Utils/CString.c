
#include "CString.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CArrayList.h"
#include "StringBuilder.h"

StringFunctions Strings = {
    .charAt = str_getCharAt,
    .combine = str_combine,
    .delete_ = str_delete,
    .clear = str_clear,
    .equals = str_equals,
    .println = str_println,
    .split = str_split,
    .startsWith = str_startsWith,
    .substring = str_substring,
    .setAll = str_setAll,
    .setCharAt = str_setCharAt,
    .fromInt_c = str_fromInt_c,
    .fromInt = str_fromInt,
    .recalculateLength = str_recalculateLength,
    .copyInto = str_setContent_c,
    .newReservedString = newReservedString,
    .appendChar = str_appendChar,
    .popChar = str_popChar,
    .isEmpty = str_isEmpty,
    .appendCharAt = str_appendCharAt
};

void str_grow(String * string, const size_t newCapacity) {
    char* oldContent = string->m;
    char* newContent;
    if (string->isHeap) {
        newContent = realloc(oldContent, newCapacity);
    } else {
        newContent = malloc(newCapacity);
        if (newContent) {
            for (int i = 0; i < string->length; i++) {
                newContent[i] = oldContent[i];
            }
            newContent[string->length] = '\0';
        }
    }
    if (newContent) {
        string->m = newContent;
        string->capacity = newCapacity;
    } else {
        puts("String growing failed, keeping old Buffer");
    }
    string->isHeap = true;
}

String stringOf(char* content){
    int length = 0;
    while (content[length] != '\0') {
        length++;
    }
    return (String){
        .m = content,
        .length = length,
        .capacity = length,
        .isHeap = false
    };
}

String newEmptyString(const size_t length) {
    char* content = malloc(sizeof(char) * length + 1);
    content[length] = '\0';

    return (String){
        .m = content,
        .length = length,
        .capacity = length,
        .isHeap = true
    };
}

String newReservedString(const size_t capacity) {
    char* content = malloc(sizeof(char) * capacity);

    return (String){
        .m = content,
        .length = 0,
        .capacity = capacity,
        .isHeap = true
    };
}

void str_setContent_c(String* string, const char* content) {
    int length = 0;
    while (content[length] != '\0') {
        length++;
    }
    if (length+1 >= string->capacity) {
        str_grow(string, length+1);
    }

    for (int i = 0; i < length; ++i) {
        string->m[i] = content[i];
    }
    string->m[length] = '\0';
    string->length = length;
}

char str_getCharAt(const String* string, const int index){
    if(index >= string->length || index < 0) return '\0';
    return string->m[index];
}

void str_setCharAt(const String* string, const int index, const char value){
    if(index >= string->length || index < 0) return;
    string->m[index] = value;
}

void str_appendChar(String* string, const char value) {
    if (string->length+1 >= string->capacity) str_grow(string, string->capacity*2 + 1);
    string->m[string->length++] = value;
    string->m[string->length] = '\0';
}

void str_appendCharAt(String* string, const char value, int index) {
    if (index < 0) index = 0;
    if (string->length+1 >= string->capacity) str_grow(string, string->capacity*2 + 1);
    size_t i = string->length;
    for (; i > index; i--) {
        string->m[i] = string->m[i-1];
    }
    string->m[i] = value;
    string->m[++string->length] = '\0';
}

char str_popChar(String* string) {
    if (string->length < string->capacity/3) str_grow(string, string->capacity/2);
    const char c = string->m[string->length--];
    string->m[string->length] = '\0';
    return c;
}

char str_popCharAt(String* string, int index) {
    if (index < 0) index = 0;
    if (string->length < string->capacity/3) str_grow(string, string->capacity/2);
    const char c = string->m[index];
    for (size_t i = index; i < string->length; i++) {
        string->m[i] = string->m[i+1];
    }
    string->m[string->length] = '\0';
    return c;
}

String str_substring(const String* string, int start_index, int end_index){
    if (start_index < 0) start_index = 0;
    if (end_index >= string->length) end_index = (int) string->length - 1;
    if (start_index > end_index) return (String){};

    const int length = (end_index - start_index) + 1;

    char* content = malloc(length + 1);
    int j = 0;
    for(int i = start_index; i <= end_index; i++){
        content[j] = string->m[i];
        j++;
    }
    content[j] = '\0';
    return (String){
        .m = content,
        .length = length,
        .capacity = length,
        .isHeap = true
    };
}

String newString_c(const char* content){
    int length = 0;
    while (content[length] != '\0') {
        length++;
    }

    char* con = malloc(sizeof(char) * (length + 1));
    for (int i = 0; i < length; i++) {
        con[i] = content[i];
    }

    con[length] = '\0';
    return (String){
        .m = con,
        .length = length,
        .capacity = length,
        .isHeap = true
    };
}

String newString_array(char* content, const size_t capacity) {
    return (String){
        .m = content,
        .length = 0,
        .capacity = capacity,
        .isHeap = false
    };
}

String str_combine(const String *string1, const String *string2) {
    const size_t length =  string1->length + string2->length;
    char* content = malloc(length + 1);
    int i;
    for (i = 0; i < string1->length; i++) {
        content[i] = string1->m[i];
    }
    for (int j = 0; j < string2->length; j++) {
        content[i+j] = string2->m[j];
    }
    content[length] = '\0';
    return (String){
        .m = content,
        .length = length,
        .capacity = length,
        .isHeap = true
    };
}

String str_fromInt_c(int value) {
    int i = 0;
    char* content = malloc(32);
    const int sign = value;

    if (value < 0) value = -value;
    while (value > 0) {
        content[i++] = (char)(value % 10 + '0');
        value /= 10;
    }

    if (sign < 0) content[i++] = '-';

    for (int j = 0, k = i - 1; j < k; j++, k--) {
        const char temp = content[j];
        content[j] = content[k];
        content[k] = temp;
    }
    content[i] = '\0';
    const String string = newString_c(content);
    free(content);
    return string;
}

void str_fromInt(char* content, const size_t size, long long value) {
    int i = 0;

    const long long sign = value;

    if (value < 0) value = -value;
    while (value > 0) {
        if (i >= size) {
            puts("Char Array is to small for str_fromInt");
            return;
        }
        content[i++] = (char)(value % 10 + '0');
        value /= 10;
    }

    if (sign < 0) content[i++] = '-';

    for (int j = 0, k = i - 1; j < k; j++, k--) {
        const char temp = content[j];
        content[j] = content[k];
        content[k] = temp;
    }
    content[i] = '\0';
}

void str_recalculateLength(String* string) {
    int length = 0;
    while (string->length != '\0') {
        length++;
    }
    string->length = length;
}

void str_delete(String* string){
    if (!string->m) return;
    if (string->isHeap)free(string->m);
    string->m = nullptr;
    string->length = 0;
    string->capacity = 0;
    string->isHeap = false;
}

void str_clear(String* string) {
    string->length = 0;
    string->m[0] = '\0';
}

void str_println(const String* string){
    printf("%s \n",string->m);
}

bool str_isEmpty(const String* s) {
    return s->length == 0;
}

bool str_equals(const String* string,const String* key){
    if(string->length != key->length) return false;

    for(int i = 0; i < string->length; i++){
        if(string->m[i] != key->m[i]) return false;
    }
    return true;
}

bool str_startsWith(const String* string, const String* key){
    if(key->length > string->length) return false;
    for(int i = 0; i < string->length; i++) {
        if(string->m[i] != key->m[i]) return false;
    }
    return true;
}

String* str_split(const String* string, const char *key) {
    const int keyLength = (int)strlen(key);

    String* strList = nullptr;
    int j = 0;
    StringBuilder sb = newStringBuilder();

    for (int i = 0; i < string->length; i++) {
        const char current = str_getCharAt(string, i);
        if (current == key[j]) {
            j++;
            if (j == keyLength) {
                arrPush(strList, sb.toString(&sb));
                j = 0;
            }
        } else {
            j = 0;
            sb.add(&sb, current);
        }
    }
    arrPush(strList, sb.toString(&sb));
    return strList;
}

void str_setAll(const String* string, const char key) {
    for (int i = 0; i < string->length; i++) {
        string->m[i] = key;
    }
}