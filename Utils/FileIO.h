//
// Created by Stefan on 22.10.2025.
//

#ifndef C_FILEIO_H
#define C_FILEIO_H
#include "CString.h"

String readFile(const String *fileName);
void writeFile(const String *fileName, const String *content);
#endif //C_FILEIO_H
