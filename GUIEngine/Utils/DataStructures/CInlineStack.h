//
// Created by Stefan on 06.10.2026.
//

#pragma once
#include <stddef.h>
#include <assert.h>

#define CInlineStack_boundsCheck(pStack) assert(pStack.size < pStack.capacity)\

#define CInlineStack_new(pName, pType, pCapacity)\
    pType pName##Data[pCapacity] = {};\
struct {\
    pType* m;\
    size_t size;\
    size_t capacity;\
} pName = {\
    .m = pName##Data,\
    .capacity = pCapacity\
}

#define CInlineStack_push(pStack, pItem)\
do {\
    CInlineStack_boundsCheck(pStack);\
    pStack.m[pStack.size++] = (pItem);\
} while(0)

#define CInlineStack_pop(pStack, pIndex)\
({\
    CInlineStack_boundsCheck(pStack);\
    pStack.m[--pStack.size];\
})
