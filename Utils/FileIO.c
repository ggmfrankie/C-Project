//
// Created by Stefan on 22.10.2025.
//

#include "FileIO.h"

#include <string.h>

#include "String.h"

String readFile(const String *fileName) {
    const String defaultShaderPath = newString("../Shader/");
    const String completePath = str_combine(&defaultShaderPath, fileName);

    FILE *file = fopen(completePath.content, "rb"); // binary mode to detect BOM
    if (!file) {
        printf("Failed to open file: %s\n", completePath.content);
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

    // Strip UTF-8 BOM if present
    if ((unsigned char)buffer[0] == 0xEF &&
        (unsigned char)buffer[1] == 0xBB &&
        (unsigned char)buffer[2] == 0xBF) {
        memmove(buffer, buffer + 3, size - 2);
        }

    return newString(buffer);
}
