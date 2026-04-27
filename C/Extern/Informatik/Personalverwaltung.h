//
// Created by ertls on 19.11.2025.
//

#ifndef C_PERSONALVERWALTUNG_H
#define C_PERSONALVERWALTUNG_H

#include "../../Utils/Deprecated/ArrayList.h"

typedef struct Personal {
    char name[64];
    int age;
    int wage;
} Personal;


void personalVerwaltung();

#endif //C_PERSONALVERWALTUNG_H