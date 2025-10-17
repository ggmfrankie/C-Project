
#include "String.h"


String newString(char* content){
    int len = 0;
    while (content[len] != '\0') {
        len++;
    }
    return (String){
        .content = content,
        .length = len,
        .charAt = str_getCharAt,
        .combine = str_combine,
        .delete = str_delete,
        .equals = str_equals,
        .println = str_println,
        .split = str_split,
        .startsWith = str_startsWith,
        .substring = str_substring
    };
}

char str_getCharAt(const String* string, const int index){
    if(index >= string->length || index < 0) return '\0';
    return string->content[index];
}

String str_substring(const String* string, int start_index, int end_index){
    if (start_index < 0) start_index = 0;
    if (end_index >= string->length) end_index = string->length - 1;
    if (start_index > end_index) return (String){};

    const int length = (end_index - start_index) + 1;

    char* content = malloc(length + 1);
    int j = 0;
    for(int i = start_index; i <= end_index; i++){
        content[j] = string->content[i];
        j++;
    }
    content[j] = '\0';
    // ReSharper disable once CppDFAMemoryLeak
    return (String){
        .content = content,
        .length = j,
        .charAt = str_getCharAt,
        .combine = str_combine,
        .delete = str_delete,
        .equals = str_equals,
        .println = str_println,
        .split = str_split,
        .startsWith = str_startsWith,
        .substring = str_substring
    };
}

String str_newString_c(const char* content){
    int len = 0;
    while (content[len] != '\0') {
        len++;
    }

    char* con = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len; i++) {
        con[i] = content[i];
    }

    con[len] = '\0';
    return (String){
        .content = con,
        .length = len,
        .charAt = str_getCharAt,
        .combine = str_combine,
        .delete = str_delete,
        .equals = str_equals,
        .println = str_println,
        .split = str_split,
        .startsWith = str_startsWith,
        .substring = str_substring
    };
}

String str_combine(const String *string1, const String *string2) {
    const int length = string1->length + string2->length;
    char* con = malloc(length + 1);
    int i;
    for (i = 0; i < string1->length; i++) {
        con[i] = string1->content[i];
    }
    for (int j = 0; j < string2->length; j++) {
        con[i+j] = string2->content[j];
    }
    con[length] = '\0';
    return (String){
        .content = con,
        .length = length,
        .charAt = str_getCharAt,
        .combine = str_combine,
        .delete = str_delete,
        .equals = str_equals,
        .println = str_println,
        .split = str_split,
        .startsWith = str_startsWith,
        .substring = str_substring
    };
}

void str_delete(const String* string){
    if (!string->content) return;
    free(string->content);
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

List_String str_split(const String* string, const String* key) {
    List_String result = {};

}


