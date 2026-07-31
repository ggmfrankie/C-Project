//
// Created by ertls on 26.11.2025.
//

#include "MemoryArena.h"

#include <stdio.h>
#include <stdlib.h>

Arena createArena(const size_t size) {
    void *memory = malloc(size);
    if (memory == NULL) {
        puts("failed to allocate memory for Arena");
    }
    return (Arena) {
        .size = size,
        .stackPointer = memory,
        .memory = memory
    };
}

void freeArena(Arena *arena) {
    if (arena == NULL) return;
    if (arena->memory) free(arena->memory);
    arena->memory = NULL;
    arena->size = 0;
    arena->stackPointer = NULL;
}

void* alloc(Arena *arena, const size_t size) {
    if (arena->size <= size) {
        puts("Not enough memory in Arena");
        return NULL;
    }
    void *allocated = arena->stackPointer;
    arena->stackPointer += size;
    arena->size -= size;
    return allocated;
}