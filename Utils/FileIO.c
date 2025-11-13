//
// Created by Stefan on 22.10.2025.
//

#include "FileIO.h"

#include <string.h>

#include "String.h"

String readFile(const String *fileName) {
    const String defaultShaderPath = stringOf("../Shader/");
    const String completePath = Strings.combine(&defaultShaderPath, fileName);

    FILE *file = fopen(completePath.content, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", completePath.content);
        exit(1);
    }
    Strings.delete(&completePath);

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
