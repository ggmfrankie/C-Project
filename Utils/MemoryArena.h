//
// Created by ertls on 26.11.2025.
//

#ifndef C_MEMORYARENA_H
#define C_MEMORYARENA_H
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

Arena createArena(size_t size);
void freeArena(Arena *arena);
void* alloc(Arena *arena, size_t size);


static ArenaFunctions Arenas = {
    .create = createArena,
    .alloc = alloc,
    .free = freeArena
};

#endif //C_MEMORYARENA_H