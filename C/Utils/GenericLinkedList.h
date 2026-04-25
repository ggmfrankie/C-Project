#pragma once


#include <stdlib.h>
#include <stdio.h>

#define DEFINE_LINKED_LIST(name, type)\
typedef struct Node_##name{\
    type* data;\
    struct Node_##name* next;\
} Node_##name;\
\
typedef struct LinkedList_##name{\
    Node_##name* head;\
    int length;\
    void (*add)(struct LinkedList_##name* list, type data);\
    type* (*get)(struct LinkedList_##name* list, unsigned int index);\
    type* (*pop)(struct LinkedList_##name* list, unsigned int index);\
    void (*delete_)(struct LinkedList_##name* list, unsigned int index);\
} LinkedList_##name;\
\
inline Node_##name* createNode_##name(type data){\
    Node_##name* node = malloc(sizeof(Node_##name));\
    node->data = malloc(sizeof(type));\
    *node->data = data;\
    node->next = NULL;\
    return node;\
}\
inline void addData_##name(LinkedList_##name* list, type data){\
    Node_##name* node = createNode_##name(data);\
    if(list->head == NULL){\
        list->head = node;\
    } else {\
        Node_##name* current = list->head;\
        while(current->next != NULL) {\
            current = current->next;\
        }\
        current->next = node;\
    }\
    list->length++;\
}\
\
inline type* getDataAt_##name(LinkedList_##name* list, unsigned int index){\
    if(index >= list->length){\
        printf("Index was out of bounds");\
        exit(1);\
        return list->head->data;\
    }\
    Node_##name* current = list->head;   \
    int i = 0;                                    \
    while (current != NULL){      \
        if(i == index) return current->data;                                       \
        current = current->next;    \
        i++;                                            \
    }                                                                           \
    return NULL;\
}                                                                               \
\
inline Node_##name* extractNode_##name(LinkedList_##name* list, unsigned int index){                \
    if(list->head == NULL || index >= list->length){                            \
        printf("List was not initialized or Index out of bounds");\
        exit(1);                                                                \
        return NULL;\
    }                                                                    \
    Node_##name* current = list->head;                                      \
    Node_##name* last = NULL;                                               \
    int i = 0;                                                              \
    while(current->next != NULL && i < index) {                             \
        last = current;                                                     \
        current = current->next;                                            \
        i++;                                                                \
    }                                                                       \
    if(last == NULL){                                                       \
        list->head = current->next;                                         \
    } else{                                                                 \
        last->next = current->next;                                         \
    }\
    list->length--;                                \
    return current;\
}\
\
inline type* pop_##name(LinkedList_##name* list, unsigned int index){\
    Node_##name* lastNode = extractNode_##name(list, index);\
    type* data = lastNode->data;\
    free(lastNode);\
    return data;\
}\
\
inline void delete_##name(LinkedList_##name* list, unsigned int index){\
    Node_##name* node = extractNode_##name(list, index);\
    free(node->data);\
    free(node);\
}\
inline LinkedList_##name newList_##name(){\
    return (LinkedList_##name){\
        .head = NULL,\
        .length = 0,\
        .add = addData_##name,\
        .get = getDataAt_##name,\
        .pop = pop_##name,\
        .delete_ = delete_##name\
    };\
}

