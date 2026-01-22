//
// Created by Stefan on 16.12.2025.
//

#ifndef C_GUIELEMENTDATA_H
#define C_GUIELEMENTDATA_H
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    float min;
    float max;

    pthread_mutex_t lock;

    float value;
} SliderData;

typedef struct {
    char text[256];

    size_t cursor;          // byte index
    size_t selectStart;     // byte index
    size_t selectEnd;       // byte index

    bool focused;
    bool selecting;

    float blinkTimer;
    bool cursorVisible;
} TextFieldData;

#endif //C_GUIELEMENTDATA_H