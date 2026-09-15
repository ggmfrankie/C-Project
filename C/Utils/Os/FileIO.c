//
// Created by Stefan on 22.10.2025.
//

#include "FileIO.h"

#include <stdio.h>
#include <stdlib.h>

#include "../_Deprecated_/CString_v1.h"
#include "../Macros/Defer.h"
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

    const Str buffer = strNew(size+1);

    fread(buffer, 1, size, file);

    buffer[size] = '\0';

    return buffer;
}
void writeFile(const char* fileName, Str content) {
    defer(defer_closeFile) FILE *file = fopen(fileName, "wb");

    if (!file) {
        WARNING_("Failed to open file: %s\n", fileName);
        return;
    }

    const size_t len = strLen(content);
    const size_t writtenChars = fwrite(content, 1, len, file);

    if (writtenChars != len) WARNING_("Failed to write complete data");
}
