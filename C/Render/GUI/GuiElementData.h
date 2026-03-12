//
// Created by Stefan on 16.12.2025.
//

#ifndef C_GUIELEMENTDATA_H
#define C_GUIELEMENTDATA_H
#include <pthread.h>

#include "../../Utils/CString.h"
struct Element;
struct Renderer;

typedef struct {
    float min;
    float max;

    pthread_mutex_t lock;

    float value;
} SliderData;

typedef struct {
    int byteIndex;
    float posX;

    float blinkTimer;
    bool cursorVisible;

    size_t selectStart;
    size_t selectEnd;
} TextFieldCursor;

typedef struct {
    String text;

    TextFieldCursor cursor;

    bool focused;
    bool selecting;

    bool (*onEnterCallback)(struct Element* element, struct Renderer *renderer);
} TextFieldData;

#endif //C_GUIELEMENTDATA_H