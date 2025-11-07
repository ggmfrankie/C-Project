
#include "String.h"
#include "StringBuilder.h"
String stringFunctionInitializer(char* content, const size_t length) {
    return (String){
        .content = content,
        .length = length,
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
}

String wrapWithString(char* content){
    int len = 0;
    while (content[len] != '\0') {
        len++;
    }
    return stringFunctionInitializer(content, len);
}

String newEmptyString(const size_t size) {
    char* content = malloc(sizeof(char) * size + 1);
    content[size] = '\0';

    return stringFunctionInitializer(content, size);
}

char str_getCharAt(const String* string, const int index){
    if(index >= string->length || index < 0) return '\0';
    return string->content[index];
}

void str_setCharAt(const String* string, const int index, const char value){
    if(index >= string->length || index < 0) return;
    string->content[index] = value;
}

String str_substring(const String* string, int start_index, int end_index){
    if (start_index < 0) start_index = 0;
    if (end_index >= string->length) end_index = (int) string->length - 1;
    if (start_index > end_index) return (String){};

    const int length = (end_index - start_index) + 1;

    char* content = malloc(length + 1);
    int j = 0;
    for(int i = start_index; i <= end_index; i++){
        content[j] = string->content[i];
        j++;
    }
    content[j] = '\0';
    return stringFunctionInitializer(content, length);
}

String newString_c(const char* content){
    int len = 0;
    while (content[len] != '\0') {
        len++;
    }

    char* con = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len; i++) {
        con[i] = content[i];
    }

    con[len] = '\0';
    return stringFunctionInitializer(con, len);
}

String str_combine(const String *string1, const String *string2) {
    const size_t length =  string1->length + string2->length;
    char* con = malloc(length + 1);
    int i;
    for (i = 0; i < string1->length; i++) {
        con[i] = string1->content[i];
    }
    for (int j = 0; j < string2->length; j++) {
        con[i+j] = string2->content[j];
    }
    con[length] = '\0';
    return stringFunctionInitializer(con, length);
}

void str_delete(const String* string){
    if (!string->content) return;
    free(string->content);
}

void str_clear(String* string) {
    string->content = NULL;
    string->length = 0;
}

void str_println(const String* string){
    printf("%s \n",string->content);
}

bool str_equals(const String* string,const String* key){
    if(string->length != key->length) return false;

    for(int i = 0; i < string->length; i++){
        if(string->content[i] != key->content[i]) return false;
    }
    return true;
}

bool str_startsWith(const String* string, const String* key){
    if(key->length > string->length) return false;
    for(int i = 0; i < string->length; i++) {
        if(string->content[i] != key->content[i]) return false;
    }
    return true;
}

List_String str_split(const String* string, const char *key) {
    int keyLength = 0;

    while (key[keyLength] != '\0') {
        keyLength++;
    }

    List_String strList = String_newList(16);
    int j = 0;
    StringBuilder sb = newStringBuilder();

    for (int i = 0; i < string->length; i++) {
        const char current = string->charAt(string, i);
        if (current == key[j]) {
            j++;
            if (j == keyLength) {
                strList.add(&strList, sb.toString(&sb));
                j = 0;
            }
        } else {
            j = 0;
            sb.add(&sb, current);
        }
    }
    strList.add(&strList, sb.toString(&sb));
    return strList;
}

void str_setAll(const String* string, const char key) {
    for (int i = 0; i < string->length; i++) {
        string->content[i] = key;
    }
}


