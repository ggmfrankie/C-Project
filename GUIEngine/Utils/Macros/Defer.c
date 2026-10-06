//
// Created by Stefan on 21.05.2026.
//

#include "Defer.h"
#include <stdio.h>
#include <stdlib.h>
#include "Utils/DataStructures/CArrayList.h"
#include "../DataStructures/Hashing/CHashTable.h"

void defer_closeFile(FILE** f) {
    if (*f) fclose(*f);
}

void defer_free(void* p) {
    void* data = *(void**)p;
    if (data) free(data);
}

void defer_arrDelete(void *a) {
    arrFree(*(void**)a);
}

void defer_CHashTableFree(void *t) {
    CHashTable_free(*(void**)t);
}
