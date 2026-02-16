//
// Created by Stefan on 22.10.2025.
//

#ifndef C_STRINGBUILDER_H
#define C_STRINGBUILDER_H
#include "CString.h"

typedef struct StringBuilder {
    char* content;
    size_t length;
    size_t capacity;
    void (*clear)(struct StringBuilder* sb);
    void (*add)(struct StringBuilder* sb, char c);
    String (*toString)(struct StringBuilder* sb);

} StringBuilder;
void StrBuilder_clear(StringBuilder *sb);
String StrBuilder_toString(StringBuilder *sb);
void StrBuilder_add(StringBuilder *sb, char c);
StringBuilder newStringBuilder();
void StrBuilder_delete(StringBuilder *sb);

#endif //C_STRINGBUILDER_H