#pragma once
#include <stddef.h>
#include "../Logging/Logging.h"
#include "../Typedef.h"
#include "../Makros/Helper.h"
typedef struct CSparseSet{
    size_t VALUE_SIZE;
    size_t* aFreeList;
    size_t numElements;
    struct {
        size_t capacity;
        ssize_t* toData;
        ssize_t* fromData;
    } indices;

    struct {
        size_t size;
        size_t capacity;
        byte* m;
    } data;
} SparseSet;

SparseSet _SparseSet_new(size_t valueSize, size_t capacity);
void SparseSet_delete(SparseSet* set);

ssize_t _SparseSet_add(SparseSet *set, const void *value);
void _SparseSet_remove(SparseSet* set, size_t id);
void _SparseSet_remove_keepOrder(SparseSet* set, size_t id);

void* _SparseSet_get(const SparseSet* set, size_t id);

#define _SparseSet_typeCheck(set, type) if ((set)->VALUE_SIZE != sizeof(type)) ERROR_("Types dont match")

#define SparseSet_new(type, capacity) _SparseSet_new(sizeof(type), capacity)

#define SparseSet_add(set, value...) ({\
    typeof(value) CONCAT(_local, __LINE__) = value;\
    _SparseSet_typeCheck(set, typeof(CONCAT(_local, __LINE__)));\
    _SparseSet_add((set), &CONCAT(_local, __LINE__));\
})

#define SparseSet_get(set, id, type) ({\
    _SparseSet_typeCheck(set, type);\
    (type*)_SparseSet_get(set, id);\
})

#define SparseSet_remove(set, id) _SparseSet_remove(set, id)
#define SparseSet_remove_keepOrder(set, id) _SparseSet_remove_keepOrder(set, id)
