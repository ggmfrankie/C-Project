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
    int size; \
    int capacity; \
} List_##name; \
\
static inline void name##_error() { \
    printf("Error in ArrayList %s processing\n", #name); \
    exit(1); \
} \
\
static inline List_##name name##_newList(int size) { \
    List_##name list; \
    list.content = malloc(size * sizeof(type)); \
    if (!list.content) name##_error(); \
        list.size = 0; \
    list.capacity = size; \
    return list; \
} \
\
static inline void name##_ListAdd(List_##name *list, type value) { \
    if (list->size >= list->capacity) { \
        type *tmp = realloc(list->content, list->capacity * 2 * sizeof(type)); \
        if (!tmp) name##_error(); \
            list->content = tmp; \
            list->capacity *= 2; \
        } \
        list->content[list->size++] = value; \
    } \
\
static inline type name##_ListGet(List_##name *list, int index) { \
    if (index < 0 || index >= list->size) name##_error(); \
    return list->content[index]; \
} \
\
static inline type* name##_ListGet_ptr(List_##name *list, int index) { \
    if (index < 0 || index >= list->size) name##_error(); \
    return &list->content[index]; \
} \
\
static inline void name##_ListFree(List_##name *list) { \
    free(list->content); \
    list->content = NULL; \
    list->size = 0; \
    list->capacity = 0; \
}

#endif // C_ARRAYLIST_H