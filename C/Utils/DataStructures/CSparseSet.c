#include "CSparseSet.h"
#include <stdlib.h>
#include <string.h>
#include "../Logging/Logging.h"

static void _SparseSet_resizeTo(SparseSet* set, size_t newCapacity) {

}

SparseSet _SparseSet_new(size_t valueSize, size_t capacity) {
    void* data = malloc(valueSize * capacity);
    if (data == nullptr) ERROR("Sparse Set allocation failed");
}

void SparseSet_delete(SparseSet* set){
    
}

size_t _SparseSet_add(SparseSet *set, void *value) {
    if (set->size == set->capacity) _SparseSet_resizeTo(set, set->capacity*2);

    size_t id = set->size++;
    memcpy(set->data + id, value, set->dataSize);
    set->indices[id] = id;
    return id;
}

void SparseSet_remove(SparseSet *set, size_t id) {
}

void SparseSet_remove_keepOrder(SparseSet *set, size_t id) {
    if (id >= set->dense.size) ERROR("Index %llu out of bounds for size %llu", id, set->dense.size);

    size_t removedIndex = set->data + id;
    if (id < set->size-1) {
        size_t lenght = (set->size - id - 1) * set->dataSize;
        
        memmove(removedIndex, removedIndex+1, lenght);
    }
    
    --set->size;

    for (size_t i = removedIndex+1; i < set->size; ++i) {
        --set->indices[i];
    }
}

void* SparseSet_get(SparseSet *set, size_t id) {
    if (id >= set->sparse.size) ERROR("Index %llu out of bounds for size %llu", id, set->sparse.size);
    return &set->dense.m[set->sparse.m[id]];
}
