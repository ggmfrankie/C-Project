//
// Created by Stefan on 25.10.2026.
//

#include <stdio.h>

#include "../../../../GUIEngine/Utils/DataStructures/CArrayList.h"

// Heinrich Schurawski
int* twoSum1(int* targets, int* nums) {
    assert(targets && nums);
    int* out = {};

    for arrEach(target, targets) {
        for (int i = 0; i < arrLen(nums); ++i) {
            const int a = nums[i];

            for (int j = i+1; j < arrLen(nums); ++j) {
                const int b = nums[j];
                const int sum = a + b;

                if (sum == *target) {
                    arrPush(out, sum);
                    goto Next;
                }
            }
        }
        Next:
    }
    return out;
}

static int comparator(const void* a, const void* b) {
    const int x = *(int *)a;
    const int y = *(int *)b;

    return (x > y) - (x < y);
}

// Devis Dinoshy
int* twoSum2(int* targets, int* nums) {
    assert(targets && nums);

    int* out = {};

    int* sorted = {};
    arrCopy(sorted, nums);

    qsort(sorted, arrLen(sorted), sizeof(*sorted), comparator);

    for arrEach(target, targets) {
        const int t = *target;

        int start = 0;
        int end = arrLen(sorted)-1;

        while (start != end) {
            const int sum = sorted[start] + sorted[end];

            if (t == sum) {
                arrPush(out, sum);
                break;
            }

            if (sum < t) --end;
            else         ++start;
        }
    }

    return out;
}

// Worst case n * m

int main() {
    printf("Hello, World\n");
}
