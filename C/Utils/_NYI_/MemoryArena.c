//
// Created by ertls on 26.11.2025.
//

#include "MemoryArena.h"

#include "../Logging/Logging.h"
#include <stdio.h>
#include <stdlib.h>

Arena Arena_create(const size_t size) {
    void *memory = malloc(size);
    if (memory == nullptr) ERROR_("failed to allocate memory for Arena");
    
    return (Arena) {
        .size = size,
        .stackPointer = memory,
        .memory = memory
    };
}

void Arena_free(Arena *arena) {
    if (arena == nullptr) return;
    if (arena->memory) free(arena->memory);
    arena->memory = nullptr;
    arena->size = 0;
    arena->stackPointer = nullptr;
}

void* Arena_alloc(Arena *arena, const size_t size) {
    if (arena->size <= size) {
        WARNING_("Not enough memory in Arena");
        return nullptr;
    }
    void *allocated = arena->stackPointer;
    arena->stackPointer += size;
    arena->size -= size;
    return allocated;
}