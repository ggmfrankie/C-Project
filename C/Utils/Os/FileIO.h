//
// Created by Stefan on 22.10.2025.
//

#pragma once
#include "../DataStructures/CString.h"
#include "GuiDefines.h"
#include "../CString_v1.h"

String readFilev1(const String *fileName);
void writeFilev1(const String *fileName, const String *content);

Str readFile(const char* fileName);
void writeFile(const char* fileName, CStr content);

