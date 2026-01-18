
#include "String.h"
#include "StringBuilder.h"

StringFunctions Strings = {
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
    .setCharAt = str_setCharAt,
    .fromInt_c = str_fromInt_c,
    .fromInt = str_fromInt,
    .recalculateLength = str_recalculateLength,
    .setContent_c = str_setContent_c,
    .newReservedString = newReservedString
};


String stringOf(char* content){
    int length = 0;
    while (content[length] != '\0') {
        length++;
    }
    return (String){
        .content = content,
        .length = length,
        .capacity = length,
        .isHeap = false
    };
}

String newEmptyString(const size_t length) {
    char* content = malloc(sizeof(char) * length + 1);
    content[length] = '\0';

    return (String){
        .content = content,
        .length = length,
        .capacity = length,
        .isHeap = true
    };
}

String newReservedString(const size_t capacity) {
    char* content = malloc(sizeof(char) * capacity);

    return (String){
        .content = content,
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
        puts("String capacity to small for str_setContent");
        return;
    }

    for (int i = 0; i < length; ++i) {
        string->content[i] = content[i];
    }
    string->content[length] = '\0';
    string->length = length;
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
    return (String){
        .content = content,
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
        .content = con,
        .length = length,
        .capacity = length,
        .isHeap = true
    };
}

String newString_array(char* content, const size_t capacity) {
    return (String){
        .content = content,
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
        content[i] = string1->content[i];
    }
    for (int j = 0; j < string2->length; j++) {
        content[i+j] = string2->content[j];
    }
    content[length] = '\0';
    return (String){
        .content = content,
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
    if (!string->content) return;
    if (string->isHeap)free(string->content);
    string->content = NULL;
    string->length = 0;
    string->capacity = 0;
    string->isHeap = false;
}

void str_clear(String* string) {
    string->content = NULL;
    string->length = 0;
    string->capacity = 0;
    string->isHeap = false;
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
        const char current = str_getCharAt(string, i);
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


