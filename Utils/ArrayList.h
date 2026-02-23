//
// Created by Stefan on 10.10.2025.
//

#ifndef C_ARRAYLIST_H
#define C_ARRAYLIST_H

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LIST(name, type) \
typedef struct List_##name { \
    type *content; \
    size_t size; \
    size_t capacity; \
    void (*add)(struct List_##name *list, type value);\
    type (*getVal)(struct List_##name *list, size_t index);\
    type* (*get)(struct List_##name *list, size_t index);\
    type* (*getLast)(struct List_##name *list);\
    type* (*getContent)(struct List_##name *list);\
    void (*free)(struct List_##name *list);\
    void (*iterator)(struct List_##name *list, void (*foo)(type *content));\
} List_##name; \
\
static inline void name##_error() { \
    printf("Error in ArrayList %s processing\n", #name); \
    exit(1); \
} \
\
static inline void name##_ListAdd(List_##name *list, type value) { \
    if(!list->content){\
        puts("List not initialized");\
        name##_error();\
    } \
    if (list->size >= list->capacity) { \
        type *tmp = (type*)realloc(list->content, list->capacity * 2 * sizeof(type) + 1); \
        if (!tmp) name##_error(); \
        list->content = tmp; \
        list->capacity *= 2; \
        } \
        list->content[list->size++] = value; \
    } \
\
static inline type name##_ListGet_val(List_##name *list, size_t index) { \
    if (index < 0 || index >= list->size) name##_error(); \
    return list->content[index]; \
} \
\
static inline type* name##_ListGet(List_##name *list, size_t index) { \
    if (index < 0 || index >= list->size) name##_error(); \
    return &list->content[index]; \
}\
\
static inline type* name##_ListGetLast(List_##name *list) { \
    return &list->content[list->size-1]; \
}\
\
static inline void name##_ListFree(List_##name *list) { \
    free(list->content); \
    list->content = NULL; \
    list->size = 0; \
    list->capacity = 0; \
}\
\
static inline void name##_ListClear(List_##name *list) { \
list->size = 0; \
}\
\
static inline void name##_iterator(List_##name *list, void (*foo)(type *content)){\
    for (size_t i = 0; i < list->size; i++) {\
        foo(name##_ListGet(list, i));\
    }\
}\
\
static inline type* name##_getContent(List_##name *list){\
    type* newContent = (type*)malloc(list->size * sizeof(type));\
    for (int i = 0; i < list->size; i++){\
        newContent[i] = list->content[i];\
    }\
    return newContent;\
}\
\
static inline List_##name name##_newList(int capacity) {\
    type* content = (type*)calloc(capacity, sizeof(type));\
    if (!content) name##_error(); \
    return (List_##name){\
        .content = content,\
        .size = 0,\
        .capacity = capacity,\
        .add = name##_ListAdd,\
        .getVal = name##_ListGet_val,\
        .get = name##_ListGet,\
        .getLast = name##_ListGetLast,\
        .free = name##_ListFree,\
        .iterator = name##_iterator,\
        .getContent = name##_getContent\
    }; \
}



#endif // C_ARRAYLIST_H