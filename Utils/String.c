

#include "String.h"

char str_getCharAt(const String* string, const int index){
    if(index >= string->length) return '-';
    return string->content[index];
}

String str_substring(const String* string, int start_index, int end_index){
    String subString = {};
    if (start_index < 0) start_index = 0;
    if (end_index >= string->length) end_index = string->length - 1;
    if (start_index > end_index) return subString;

    const int length = (end_index - start_index) + 1;

    char* content = malloc(length + 1);
    int j = 0;
    for(int i = start_index; i <= end_index; i++){
        content[j] = string->content[i];
        j++;
    }
    content[j] = '\0';
    subString.content = content;
    subString.length = j;
    return subString;
}

String str_newString(char* content){
    String result;
    result.content = content;

    int len = 0;
    while (content[len] != '\0') {
        len++;
    }
    result.length = len;
    return result;
}

String str_newString_c(const char* content){
    String result;
    int len = 0;

    while (content[len] != '\0') {
        len++;
    }
    result.length = len;
    result.content = malloc(sizeof(char) * (len + 1));

    for (int i = 0; i < len; i++) {
        result.content[i] = content[i];
    }

    result.content[len] = '\0';
    return result;
}

String str_combine(const String *string1, const String *string2) {
    String result;
    result.length = string1->length+string2->length;
    result.content = malloc(result.length + 1);
    int i;
    for (i = 0; i < string1->length; i++) {
        result.content[i] = string1->content[i];
    }
    for (int j = 0; j < string2->length; j++) {
        result.content[i+j] = string2->content[j];
    }
    result.content[result.length] = '\0';
    return result;
}

/*
String newString_c(const int capacity){
    String result;
    result.content = malloc((capacity + 1)*sizeof(char));
    result.content[0] = '\0';
    result.length = 0;
    result.capacity = capacity;
    return result;
}
*/

void str_delete(const String* string){
    free(string->content);
}

void str_println(const String* string){
    printf("%s \n",string->content);
}

bool str_equals(const String* string,const String* key){
    char* str1 = string->content;
    char* str2 = key->content;

    if(string->length != key->length) return false;

    for(int i = 0; i < string->length; i++){
        if(str1[i] != str2[i]) return false;
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


