//
// Created by Stefan on 06.10.2026.
//

#pragma once
#include <stddef.h>
#include <assert.h>

#define CInlineStack_new(pName, pType, pCapacity)\
struct {\
    pType m[pCapacity];\
    size_t size;\
} pName = {}

#define CInlineStack_push(pStack, pItem)\
do {\
    assert(pStack.size < sizeof(pStack.m));\
    pStack.m[pStack.size++] = (pItem);\
} while(0)

#define CInlineStack_pop(pStack, pIndex)\
({\
    assert(pIndex < sizeof(pStack.m));\
    pStack.m[--pStack.size];\
})
