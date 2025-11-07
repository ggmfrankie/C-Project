//
// Created by Stefan on 04.11.2025.
//

#ifndef C_MATRIX_H
#define C_MATRIX_H
#include <stdlib.h>

typedef int type;
typedef int name;

typedef struct name_matrix_s {
    type** content;
    size_t sizeX;
    size_t sizeY;
} name_matrix;

#endif //C_MATRIX_H