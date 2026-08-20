//
// Created by Stefan on 21.05.2026.
//

#include "Defer.h"
#include <stdio.h>
#include <stdlib.h>
#include "../DataStructures/CString.h"
#include "Utils/DataStructures/CArrayList.h"

void defer_closeFile(FILE** f) {
    if (*f) fclose(*f);
}

void defer_free(void* p) {
    free(*(void**)p);
}

void defer_strDelete(CStr* s) {
    strDelete((Str)*s);
}

void defer_arrDelete(void *a) {
    arrDelete(a);
}
