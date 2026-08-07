//
// Created by ertls on 31.07.2026.
//

#pragma once
#include <stdio.h>
#define ERROR_(fms, ...) do {fprintf(stderr, "[ERROR] File: "__FILE__" Line: %i\n"fms"\n", __LINE__, ##__VA_ARGS__); abort();} while(0)
#define WARNING_(fms, ...) do {fprintf(stderr, "[WARNING] File: "__FILE__" Line: %i\n"fms"\n", __LINE__, ##__VA_ARGS__);} while(0)
#define TODO_(fms, ...) fprintf(stderr, "[TODO] File: "__FILE__" Line: %i\n"fms"\n", __LINE__, ##__VA_ARGS__);
#define INFO_(fms, ...) fprintf(stdout, "[INFO]: "fms"\n", ##__VA_ARGS__);