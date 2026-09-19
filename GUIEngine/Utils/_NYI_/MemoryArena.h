//
// Created by ertls on 26.11.2025.
//

#pragma once
#include "GuiDefines.h"
#include <stddef.h>

typedef struct {
    size_t size;
    void *memory;
    void *stackPointer;
} Arena;

typedef struct {
    void* (*alloc)(Arena *arena, size_t size);
    void (*free)(Arena * arena);
    Arena (*create)(size_t size);
} ArenaFunctions;

Arena Arena_create(size_t size);
void Arena_free(Arena *arena);
void* Arena_alloc(Arena *arena, size_t size);


static ArenaFunctions Arenas = {
    .create = Arena_create,
    .alloc = Arena_alloc,
    .free = Arena_free
};
