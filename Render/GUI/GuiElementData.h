//
// Created by Stefan on 16.12.2025.
//

#ifndef C_GUIELEMENTDATA_H
#define C_GUIELEMENTDATA_H
#include <pthread.h>

typedef struct {
    float min;
    float max;

    pthread_mutex_t lock;

    float value;
} SliderData;

#endif //C_GUIELEMENTDATA_H