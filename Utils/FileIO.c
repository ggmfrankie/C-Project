//
// Created by Stefan on 22.10.2025.
//

#include "FileIO.h"

#include <string.h>

#include "String.h"

String readFile(const String *fileName) {

    FILE *file = fopen(fileName->content, "rb");
    if (!file) {
        printf("Failed to open file: %s\n", fileName->content);
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
