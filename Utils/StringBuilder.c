//
// Created by Stefan on 22.10.2025.
//

#include "StringBuilder.h"



StringBuilder newStringBuilder() {
    return (StringBuilder){
        .content = NULL,
        .length = 0,
        .capacity = 0,
        .clear = StrBuilder_clear,
        .add = StrBuilder_add,
        .toString = StrBuilder_toString
    };
}

void StrBuilder_add(StringBuilder *sb, const char c) {
    if (sb->content == NULL) {
        sb->content = malloc(16);
        sb->capacity = 16;
        sb->length = 0;
    } else if (sb->capacity <= sb->length + 1) {
        char *newContent = realloc(sb->content, sb->capacity*2);
        if (newContent != NULL) {
            sb->content = newContent;
            sb->capacity = sb->capacity*2;
        }
    }

    sb->content[sb->length++] = c;
}

String StrBuilder_toString(StringBuilder *sb) {
    char *newContent = realloc(sb->content, sb->length+1);
    newContent[sb->length] = '\0';
    sb->clear(sb);
    return stringOf(newContent);
}

void StrBuilder_clear(StringBuilder *sb) {
    sb->content = NULL;
    sb->capacity = 0;
    sb->length = 0;
}

void StrBuilder_delete(StringBuilder *sb) {
    free(sb->content);
    sb->content = NULL;
    sb->capacity = 0;
    sb->length = 0;
}
