//
// Created by Stefan on 22.10.2025.
//

#include "FileIO.h"

#include <stdio.h>
#include <stdlib.h>

#include "../_Deprecated_/CString_v1.h"
#include "../Makros/Defer.h"
#include "Utils/Logging/Logging.h"

Str readFile(const char* fileName) {
    defer(defer_closeFile) FILE *file = fopen(fileName, "rb");

    if (!file) {
        WARNING_("Failed to open file: %s\n", fileName);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    rewind(file);

    Str buffer = strNew_c(size+1);

    fread(buffer, 1, size, file);

    buffer[size] = '\0';

    return buffer;
}
void writeFile(const char* fileName, CStr content) {
    defer(defer_closeFile) FILE *file = fopen(fileName, "wb");

    if (!file) {
        WARNING_("Failed to open file: %s\n", fileName);
        return;
    }

    const size_t len = strLen(content);
    const size_t writtenChars = fwrite(content, 1, len, file);

    if (writtenChars != len) WARNING_("Failed to write complete data");
}

String readFilev1(const String *fileName) {

    FILE *file = fopen(fileName->m, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", fileName->m);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    rewind(file);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        fclose(file);
        exit(1);
    }

    fread(buffer, 1, size, file);
    fclose(file);

    buffer[size] = '\0';

    return stringOf(buffer);
}

void writeFilev1(const String *fileName, const String *content) {
    FILE *file = fopen(fileName->m, "wb");
    const size_t writtenChars = fwrite(content->m, 1, content->length, file);
    if (writtenChars != content->length) {
        perror("Failed to write complete data");
        fclose(file);
        exit(1);
    }

    fclose(file);
}
