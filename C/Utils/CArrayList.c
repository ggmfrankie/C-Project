//
// Created by ertls on 27.02.2026.
//
#include "CArrayList.h"

void arrText() {

    float* (arr) = nullptr;

    arrPush(arr, 19);
    arrPush(arr, 7.0);

    for (int i = 0; i < arrLen(arr); i++) {
        printf("%f\n", arr[i]);
    }

    for_eachArr(num, arr,
        printf("%f\n", *num);
    );

    for_eachRevArr(num, arr,
        printf("%f\n", *num);
    );

}