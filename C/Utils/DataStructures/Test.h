//
// Created by ertls on 27.02.2026.
//
#pragma once

#include "CArrayList.h"
#include "CHashMap.h"

static void foo(char c[static 1]){
    puts(c);
}

static void Utils_test() {
#undef _HashMapInitCapacity_
#define _HashMapInitCapacity_ 2
    int* hTest = nullptr;

    puts("insert");
    mapInsert(hTest, "no", 7);
    mapInsert(hTest, "haile", 12);
    mapInsert(hTest, "baba", 69420);

    puts("getting");
    printf("entries: %i, %i, %i\n", *mapGet(hTest, "no"), *mapGet(hTest, "haile"), *mapGet(hTest, "baba"));

    return;
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

    int a = ({int x = 9; int y = 10; x + y;});

    float $c= *arrTryGet(arr, 8);

    switch(a){
        case 0 ... 3:
            puts("holla");
        break;
        default: 

    };

    foo("nullptr");

    int x = 'FOO!';
}


