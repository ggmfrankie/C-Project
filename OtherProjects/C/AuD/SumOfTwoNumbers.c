//
// Created by ggmfrankie on 10/8/26.
//

#include "SumOfTwoNumbers.h"
#include "Utils/DataStructures/CArrayList.h"
#include "Utils/Macros/Defer.h"
#include "Utils/Os/Time.h"

static int comparator(const void* a, const void* b) {
    const int x = *(int *)a;
    const int y = *(int *)b;

    return (x > y) - (x < y);
}

static int* binarySearch(int* start, int* pivotPtr, int* end, int value) {
    const int pivot = *pivotPtr;

    if (value == pivot) {
        return pivotPtr;
    }

    if (end - start < 2) return nullptr;

    if (value < pivot) {
        const int half = (pivotPtr - start) / 2;
        int* newPivot = pivotPtr - half;
        return binarySearch(start, newPivot, pivotPtr, value);
    }
    else {
        const int half = (end - pivotPtr) / 2;
        int* newPivot = pivotPtr + half;
        return binarySearch(pivotPtr, newPivot, end, value);
    }
}

int* SumTwo_calculate(int* aNums, int* aTargets) {
    int* aOut = {};
    assert(aNums && aTargets);
    const int len = arrLen(aNums);

    qsort(aNums, len, sizeof(*aNums), comparator);
    const int min = aNums[0];
    const int max = *arrPeek(aNums);

    for arrEach(target, aTargets) {
        for arrEach(num, aNums) {
            const int remainder = *target - *num;
            if (remainder < min || remainder > max) continue;
            const int* it = binarySearch(aNums, aNums + len/2, aNums + len, remainder);
            if (it) {
                //printf("Target: %i = %i + %i\n", *target, *num, *it);
                arrPush(aOut, *target);
                goto Skip;
            }
        }
        Skip:
    }

    return aOut;
}

void SumTwo_test() {


    defer(defer_arrDelete) int* aNums = {};
    defer(defer_arrDelete) int* aTargets = {};

    for (int i = 0; i < 100; ++i) {
        arrPush(aNums, rand()%1000);
    }

    for (int i = 0; i < 10000; ++i) {
        arrPush(aTargets, rand()%2000);
    }

    const uint64_t startTime = Time_nowNs();
    defer(defer_arrDelete) int* out = SumTwo_calculate(aNums, aTargets);

    printf("Matches = %llu\nTime elapsed: %llu ms\n", arrLen(out), (Time_nowNs()-startTime) / 1000000);
}
