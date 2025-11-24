//
// Created by ertls on 19.11.2025.
//

#ifndef C_PERSONALVERWALTUNG_H
#define C_PERSONALVERWALTUNG_H

#include "../../Utils/ArrayList.h"

typedef struct Personal {
    char name[64];
    int age;
    int wage;
} Personal;

ARRAY_LIST(Personal, Personal)

void personalVerwaltung();

#endif //C_PERSONALVERWALTUNG_H