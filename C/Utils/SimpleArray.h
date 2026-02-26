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
} name##_Array;\
\
static inline void name##_Array_set(const name##_Array* array, const size_t index, const type content) {\
    if (index >= array->size) return;\
    array->content[index] = content;\
}\
\
static inline type* name##_Array_get(const name##_Array* array, const size_t index) {\
    if (index >= array->size) return NULL;\
    return &array->content[index];\
}\
\
static inline type name##_Array_getValue(const name##_Array* array, const size_t index) {\
    if (index >= array->size) exit(0);\
    return array->content[index];\
}\
\
static inline void name##_Array_setAll(name##_Array* array, const type value){\
    for (int i = 0; i < array->size; i++) {\
        array->content[i] = value;\
    }\
}\
\
static inline void name##_Array_delete(name##_Array* array) {\
    free(array->content);\
    array->size = 0;\
    array->content = NULL;\
}\
\
static inline name##_Array name##_newArray(type* content, size_t size){\
    return (name##_Array){\
        .content = content,\
        .size = size\
    };\
}


#endif //C_ARRAY_H