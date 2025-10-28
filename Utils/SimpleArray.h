//
// Created by Stefan on 28.10.2025.
//

#ifndef C_ARRAY_H
#define C_ARRAY_H
#include <stdlib.h>



#define SIMPLEARRAY(name, type)\
typedef struct name##_Array {\
    type *content;\
    size_t size;\
    const size_t capacity;\
} name##_Array;\
\
static inline void name##_Array_set(const name##_Array* array, const size_t index, const type content) {\
    if (index >= array->size) return;\
    array->content[index] = content;\
}\
\
static inline type* name##_Array_get(const name##_Array* array, const size_t index) {\
    if (index >= array->capacity) return NULL;\
    return &array->content[index];\
}\
\
static inline type name##_Array_getValue(const name##_Array* array, const size_t index) {\
    if (index >= array->capacity) exit(0);\
    return array->content[index];\
}\
\
static inline void name##_Array_add(name##_Array* array, const type content) {\
    if (array->size >= array->capacity) {\
        printf("Array is full");\
        return;\
    }\
    array->content[array->size] = content;\
    array->size++;\
}\
\
static inline name##_Array name##_newArray(type* content, size_t capacity, size_t size){\
    return (name##_Array){\
        .content = content,\
        .capacity = capacity,\
        .size = size\
    };\
}


#endif //C_ARRAY_H