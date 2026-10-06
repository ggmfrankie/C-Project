//
// Created by Stefan on 06.10.2026.
//

#include "CInlineStack.h"


#include <stddef.h>
typedef int pType;
static void test() {
    CInlineStack_new(pStack, int, 16);

    CInlineStack_push(pStack, 12);

    int i = CInlineStack_pop(pStack, 0);
}