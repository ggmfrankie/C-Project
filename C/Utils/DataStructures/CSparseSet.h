#pragma once
#include <stddef.h>
#include "../Logging/Logging.h"
#include "../UtilsTypedef.h"
typedef struct CSparseSet{
    size_t dataSize;
    size_t* aFreeList;
    struct {
        size_t size;
        ssize_t* m;
    } sparse;

    struct {
        size_t size;
        size_t capacity;
        byte* m;
    } dense;
} SparseSet;

SparseSet _SparseSet_new(size_t valueSize, size_t capacity);

size_t _SparseSet_add(SparseSet* set, void* value);
void SparseSet_remove(SparseSet* set, size_t index);

void* SparseSet_get(SparseSet* set, size_t index);

#define SparseSet_new(type, capacity) _SparseSet_new(sizeof(type), capacity)
#define SparseSet_add(set, value) ({\
    if ((set)->dataSize != sizeof(value)) ERROR("Types dont match");\
    _SparseSet_add((set), (byte*)&(value));\
})
