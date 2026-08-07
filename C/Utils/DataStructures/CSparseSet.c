#include "CSparseSet.h"
#include <stdlib.h>
#include <string.h>
#include "../Logging/Logging.h"
#include "CArrayList.h"

static void _SparseSet_resizeIndices(SparseSet* set, size_t newCapacity){
    ssize_t* toData = realloc(set->indices.toData, newCapacity * sizeof(ssize_t));
    ssize_t* fromData = realloc(set->indices.fromData, newCapacity * sizeof(ssize_t));
    if (toData == nullptr || fromData == nullptr) ERROR_("Reallocation failed");
    set->indices.toData = toData;
    set->indices.fromData = fromData;
    set->indices.capacity = newCapacity;
}   

static void _SparseSet_resizeData(SparseSet* set, size_t newCapacity) {
    void* newData = realloc(set->data.m, newCapacity * set->VALUE_SIZE);
    if (newData == nullptr) ERROR_("Reallocation failed");
    set->data.m = newData;
    set->data.capacity = newCapacity;
}

static int _getOffset(SparseSet* set, int idx){
    return idx * set->VALUE_SIZE;
}

SparseSet _SparseSet_new(size_t valueSize, size_t capacity) {
    void* data = malloc(capacity * valueSize);
    if (data == nullptr) goto Error;

    ssize_t* indices = malloc(capacity * sizeof(ssize_t));
    memset(indices, -1, capacity * sizeof(ssize_t));
    if (indices == nullptr) goto Error;

    ssize_t* reverseIndices = malloc(capacity * sizeof(ssize_t));
    memset(reverseIndices, -1, capacity * sizeof(ssize_t));
    if (reverseIndices == nullptr) goto Error;

    return (SparseSet){
        .VALUE_SIZE = valueSize,
        .aFreeList = nullptr,
        .data = {.size = 0, .capacity = capacity, .m = data}, 
        .indices = {.capacity = capacity, .toData = indices, .fromData = reverseIndices}
    };

    Error:
    ERROR_("Sparse Set allocation failed");
}

void SparseSet_delete(SparseSet* set){
    free(set->data.m);
    free(set->indices.toData);
    free(set->indices.fromData);
    arrDelete(set->aFreeList);
    memset(set, 0, sizeof(SparseSet));
}

size_t _SparseSet_add(SparseSet *set, void *value) {
    size_t sizeData = set->data.size;
    size_t id;
    if (arrIsEmpty(set->aFreeList)) {
        if (set->numElements == set->indices.capacity) {
            _SparseSet_resizeIndices(set, set->indices.capacity*2);
        }
        id = set->numElements++;
    } else {
        id = arrPop(set->aFreeList);
    }

    if (set->data.size == set->data.capacity) _SparseSet_resizeData(set, set->data.capacity*2);
    size_t index = set->data.size++;
    memcpy(&set->data.m[_getOffset(set, index)], value, set->VALUE_SIZE);

    set->indices.toData[id] = index;
    set->indices.fromData[index] = id;
    return id;
}

void _SparseSet_remove(SparseSet *set, size_t id) {
    TODO_("Implement");
}

void _SparseSet_remove_keepOrder(SparseSet *set, size_t id) {
    if (id >= set->indices.capacity) ERROR_("Index %llu out of bounds for size %llu", id, set->indices.capacity);
    if (set->indices.toData[id] == -1) ERROR_("Id %llu already freed", id);

    const size_t removedIndex = set->indices.toData[id];
    set->indices.toData[id] = -1;
    set->indices.fromData[removedIndex] = -1;

    const size_t length = (set->data.size - removedIndex - 1);
    if (removedIndex < set->data.size-1) {
        memmove(
            set->data.m + _getOffset(set, removedIndex), 
            set->data.m + _getOffset(set, removedIndex+1), 
            length * set->VALUE_SIZE
        );

        memmove(
            set->indices.fromData + (removedIndex), 
            set->indices.fromData + (removedIndex+1),
            length * sizeof(ssize_t)
        );
    }
    
    --set->data.size;

    for (size_t i = removedIndex; i < set->data.size; ++i) {
        --set->indices.toData[set->indices.fromData[i]];
    }

    --set->numElements;
    arrPush(set->aFreeList, id);
}

void* _SparseSet_get(SparseSet *set, size_t id) {
    if (id >= set->indices.capacity) ERROR_("Index %llu out of bounds for size %llu", id, set->numElements);
    return &set->data.m[set->indices.toData[id] * set->VALUE_SIZE];
}
