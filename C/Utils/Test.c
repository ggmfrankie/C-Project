#include "Test.h"
#include "DataStructures/CArrayList.h"
#include "DataStructures/CHashMap.h"
#include "DataStructures/CSparseSet.h"
#include "DataStructures/CString.h"
#include <assert.h>
#include <stdio.h>

#define TEST(condition, msg...) if(!(condition)) ERROR_(msg)


// Enable to manually verify aborting behavior (double-remove / get-removed-id).
// Disabled by default because ERROR_ aborts the whole test run.
#define RUN_ABORTING_SPARSESET_TESTS 0

static void Test_sparseSet_edgeCases() {
    SparseSet set = SparseSet_new(int, 2);

    // Fill initial capacity.
    size_t id0 = SparseSet_add(&set, 100);
    size_t id1 = SparseSet_add(&set, 200);
    TEST(id0 == 0, "id0 expected 0, got %zu", id0);
    TEST(id1 == 1, "id1 expected 1, got %zu", id1);

    // Growth case: adding a third element should keep mappings/data valid.
    size_t id2 = SparseSet_add(&set, 300);
    TEST(id2 == 2, "id2 expected 2 after growth, got %zu", id2);
    TEST(set.numElements == 3, "numElements expected 3 after growth insert, got %zu", set.numElements);
    TEST(set.data.size == 3, "data.size expected 3 after growth insert, got %zu", set.data.size);

    TEST(*SparseSet_get(&set, 0, int) == 100, "id0 value expected 100, got %d", *SparseSet_get(&set, 0, int));
    TEST(*SparseSet_get(&set, 1, int) == 200, "id1 value expected 200, got %d", *SparseSet_get(&set, 1, int));
    TEST(*SparseSet_get(&set, 2, int) == 300, "id2 value expected 300, got %d", *SparseSet_get(&set, 2, int));

    // Remove middle id, verify order and index-array maintenance.
    SparseSet_remove_keepOrder(&set, 1);
    TEST(set.numElements == 2, "numElements expected 2 after removing id 1, got %zu", set.numElements);
    TEST(set.data.size == 2, "data.size expected 2 after removing id 1, got %zu", set.data.size);

    int* dense = (int*)set.data.m;
    TEST(dense[0] == 100, "dense[0] expected 100 after middle remove, got %d", dense[0]);
    TEST(dense[1] == 300, "dense[1] expected 300 after middle remove, got %d", dense[1]);

    TEST(set.indices.toData[0] == 0, "toData[0] expected 0 after middle remove, got %zd", set.indices.toData[0]);
    TEST(set.indices.toData[1] == -1, "toData[1] expected -1 (removed), got %zd", set.indices.toData[1]);
    TEST(set.indices.toData[2] == 1, "toData[2] expected 1 after middle remove, got %zd", set.indices.toData[2]);

    TEST(set.indices.fromData[0] == 0, "fromData[0] expected 0 after middle remove, got %zd", set.indices.fromData[0]);
    TEST(set.indices.fromData[1] == 2, "fromData[1] expected 2 after middle remove, got %zd", set.indices.fromData[1]);

    TEST(arrLen(set.aFreeList) == 1, "free-list length expected 1 after one remove, got %zu", arrLen(set.aFreeList));
    TEST(*arrGetLast(set.aFreeList) == 1, "free-list last expected removed id 1, got %zu", *arrGetLast(set.aFreeList));

    // Reinsert should reuse a free id.
    size_t reused = SparseSet_add(&set, 250);
    TEST(reused == 1, "reused id expected 1 from free-list, got %zu", reused);
    TEST(*SparseSet_get(&set, 1, int) == 250, "reused id 1 value expected 250, got %d", *SparseSet_get(&set, 1, int));

#if RUN_ABORTING_SPARSESET_TESTS
    // Expected to abort in a defensive implementation.
    SparseSet_remove_keepOrder(&set, 1);
    SparseSet_remove_keepOrder(&set, 1); // double remove should be rejected
    SparseSet_get(&set, 1, int);         // getting removed id should be rejected
#endif

    SparseSet_delete(&set);
}

static void Test_arrayList() {
    int* array = nullptr;

    arrPush(array, 0);
    TEST(array[0] == 0, "array[0] expected 0, got %d", array[0]);
    TEST(*arrGetLast(array) == 0, "arrGetLast expected 0, got %d", *arrGetLast(array));

    for (int i = 1; i < 10; ++i){
        arrPush(array, i);
    }
    TEST(arrLen(array) == 10, "arrLen expected 10 after pushes, got %zu", arrLen(array));

    int a = arrPop(array);
    TEST(a == 9, "arrPop expected 9, got %d", a);
    TEST(arrLen(array) == 9, "arrLen expected 9 after pop, got %zu", arrLen(array));

    int sum = 0;
    for_eachArr(num, array, {
        sum += *num;
    });
    TEST(0+1+2+3+4+5+6+7+8 == sum, "forward sum mismatch, expected %d got %d", 0+1+2+3+4+5+6+7+8, sum);

    sum = 0;
    for_eachRevArr(num, array, {
        sum += *num;
    });
    TEST(0+1+2+3+4+5+6+7+8 == sum, "reverse sum mismatch, expected %d got %d", 0+1+2+3+4+5+6+7+8, sum);

    arrClear(array);
    TEST(_arrayGetHead(array)->capacity >= 9, "capacity expected >= 9 after clear, got %zu", _arrayGetHead(array)->capacity);
    TEST(_arrayGetHead(array)->size == 0, "size expected 0 after clear, got %zu", _arrayGetHead(array)->size);

    int* res = arrTryGet(array, 100);
    TEST(res == nullptr, "arrTryGet out-of-range expected nullptr");

    arrDelete(array);
}

static void Test_hashMap() {
    int* map = nullptr;
    TEST(mapEmpty(map) == true, "new map should be empty");

    mapInsert(map, "one", 1);
    TEST(*mapGet(map, "one") == 1, "map['one'] expected 1, got %d", *mapGet(map, "one"));

    mapInsert(map, "two", 2);
    TEST(*mapGet(map, "two") == 2, "map['two'] expected 2, got %d", *mapGet(map, "two"));
    TEST(mapLen(map) == 2, "mapLen expected 2, got %zu", mapLen(map));

    // Missing key should return nullptr.
    TEST(mapGet(map, "does-not-exist") == nullptr, "missing key should return nullptr");

    // Stress insertion and retrieval with stable key buffers.
    enum { extraEntries = 300 };
    char keys[extraEntries][32];
    for (int i = 0; i < extraEntries; ++i) {
        snprintf(keys[i], sizeof(keys[i]), "k_%d", i);
        mapInsert(map, keys[i], i * 3);
    }

    TEST(mapLen(map) == (size_t)(2 + extraEntries), "mapLen expected %zu after bulk insert, got %zu", (size_t)(2 + extraEntries), mapLen(map));
    TEST(mapCap(map) > 256, "mapCap expected growth beyond 256, got %zu", mapCap(map));

    for (int i = 0; i < extraEntries; ++i) {
        int* value = mapGet(map, keys[i]);
        TEST(value != nullptr, "value for key '%s' should not be nullptr", keys[i]);
        TEST(*value == i * 3, "value for key '%s' expected %d got %d", keys[i], i * 3, value ? *value : -1);
    }

    // Original entries must still be retrievable after rehash growth.
    TEST(*mapGet(map, "one") == 1, "map['one'] changed after growth, got %d", *mapGet(map, "one"));
    TEST(*mapGet(map, "two") == 2, "map['two'] changed after growth, got %d", *mapGet(map, "two"));

    mapDelete(map);
}

static void Test_sparseSet_removeKeepOrder() {
    SparseSet set = SparseSet_new(int, 16);

    // Dense values start in order 10..15, ids are 0..5.
    for (int i = 0; i < 6; ++i) {
        size_t id = SparseSet_add(&set, 10 + i);
        TEST(id == (size_t)i, "initial id mismatch at i=%d expected %zu got %zu", i, (size_t)i, id);
    }

    // Remove middle id (2) and verify dense order + index maps.
    SparseSet_remove_keepOrder(&set, 2);
    TEST(set.data.size == 5, "data.size expected 5 after removing middle id, got %zu", set.data.size);
    TEST(set.numElements == 5, "numElements expected 5 after removing middle id, got %zu", set.numElements);

    int* dense = (int*)set.data.m;
    TEST(dense[0] == 10, "dense[0] expected 10, got %d", dense[0]);
    TEST(dense[1] == 11, "dense[1] expected 11, got %d", dense[1]);
    TEST(dense[2] == 13, "dense[2] expected 13, got %d", dense[2]);
    TEST(dense[3] == 14, "dense[3] expected 14, got %d", dense[3]);
    TEST(dense[4] == 15, "dense[4] expected 15, got %d", dense[4]);

    TEST(set.indices.toData[0] == 0, "toData[0] expected 0, got %zd", set.indices.toData[0]);
    TEST(set.indices.toData[1] == 1, "toData[1] expected 1, got %zd", set.indices.toData[1]);
    TEST(set.indices.toData[3] == 2, "toData[3] expected 2, got %zd", set.indices.toData[3]);
    TEST(set.indices.toData[4] == 3, "toData[4] expected 3, got %zd", set.indices.toData[4]);
    TEST(set.indices.toData[5] == 4, "toData[5] expected 4, got %zd", set.indices.toData[5]);

    TEST(set.indices.fromData[0] == 0, "fromData[0] expected 0, got %zd", set.indices.fromData[0]);
    TEST(set.indices.fromData[1] == 1, "fromData[1] expected 1, got %zd", set.indices.fromData[1]);
    TEST(set.indices.fromData[2] == 3, "fromData[2] expected 3, got %zd", set.indices.fromData[2]);
    TEST(set.indices.fromData[3] == 4, "fromData[3] expected 4, got %zd", set.indices.fromData[3]);
    TEST(set.indices.fromData[4] == 5, "fromData[4] expected 5, got %zd", set.indices.fromData[4]);

    // Remove first surviving id (0) and re-check order/maps.
    SparseSet_remove_keepOrder(&set, 0);
    TEST(set.data.size == 4, "data.size expected 4 after removing first id, got %zu", set.data.size);
    TEST(set.numElements == 4, "numElements expected 4 after removing first id, got %zu", set.numElements);

    dense = (int*)set.data.m;
    TEST(dense[0] == 11, "dense[0] expected 11 after removing first, got %d", dense[0]);
    TEST(dense[1] == 13, "dense[1] expected 13 after removing first, got %d", dense[1]);
    TEST(dense[2] == 14, "dense[2] expected 14 after removing first, got %d", dense[2]);
    TEST(dense[3] == 15, "dense[3] expected 15 after removing first, got %d", dense[3]);

    TEST(set.indices.toData[1] == 0, "toData[1] expected 0 after removing first, got %zd", set.indices.toData[1]);
    TEST(set.indices.toData[3] == 1, "toData[3] expected 1 after removing first, got %zd", set.indices.toData[3]);
    TEST(set.indices.toData[4] == 2, "toData[4] expected 2 after removing first, got %zd", set.indices.toData[4]);
    TEST(set.indices.toData[5] == 3, "toData[5] expected 3 after removing first, got %zd", set.indices.toData[5]);

    TEST(set.indices.fromData[0] == 1, "fromData[0] expected 1 after removing first, got %zd", set.indices.fromData[0]);
    TEST(set.indices.fromData[1] == 3, "fromData[1] expected 3 after removing first, got %zd", set.indices.fromData[1]);
    TEST(set.indices.fromData[2] == 4, "fromData[2] expected 4 after removing first, got %zd", set.indices.fromData[2]);
    TEST(set.indices.fromData[3] == 5, "fromData[3] expected 5 after removing first, got %zd", set.indices.fromData[3]);

    // Remove id 5 (currently last dense entry) and verify state.
    SparseSet_remove_keepOrder(&set, 5);
    TEST(set.data.size == 3, "data.size expected 3 after removing last id, got %zu", set.data.size);
    TEST(set.numElements == 3, "numElements expected 3 after removing last id, got %zu", set.numElements);

    dense = (int*)set.data.m;
    TEST(dense[0] == 11, "dense[0] expected 11 at end, got %d", dense[0]);
    TEST(dense[1] == 13, "dense[1] expected 13 at end, got %d", dense[1]);
    TEST(dense[2] == 14, "dense[2] expected 14 at end, got %d", dense[2]);

    TEST(set.indices.toData[1] == 0, "toData[1] expected 0 at end, got %zd", set.indices.toData[1]);
    TEST(set.indices.toData[3] == 1, "toData[3] expected 1 at end, got %zd", set.indices.toData[3]);
    TEST(set.indices.toData[4] == 2, "toData[4] expected 2 at end, got %zd", set.indices.toData[4]);

    TEST(set.indices.fromData[0] == 1, "fromData[0] expected 1 at end, got %zd", set.indices.fromData[0]);
    TEST(set.indices.fromData[1] == 3, "fromData[1] expected 3 at end, got %zd", set.indices.fromData[1]);
    TEST(set.indices.fromData[2] == 4, "fromData[2] expected 4 at end, got %zd", set.indices.fromData[2]);

    SparseSet_delete(&set);
}

static void Test_sparseSet(){
    SparseSet set = SparseSet_new(int, 16);
    size_t firstId = SparseSet_add(&set, 12);
    TEST(firstId == 0, "first sparse-set id expected 0, got %zu", firstId);

    int* a = SparseSet_get(&set, 0, int);
    TEST(*a == 12, "value at id 0 expected 12, got %d", *a);

    // Fill to capacity and verify dense index -> value mapping stays correct.
    for (int i = 1; i < 16; i++) {
        size_t id = SparseSet_add(&set, i);
        TEST(id == (size_t)i, "insert id mismatch at i=%d expected %zu got %zu", i, (size_t)i, id);
    }

    for (int i = 0; i < 16; i++) {
        int* value = SparseSet_get(&set, (size_t)i, int);
        TEST(*value == i || (i == 0 && *value == 12), "value mismatch at id=%d got %d", i, *value);
    }

    // Existing values should be mutable through returned pointer.
    int* writable = SparseSet_get(&set, 5, int);
    *writable = 42;
    TEST(*SparseSet_get(&set, 5, int) == 42, "mutated value at id 5 expected 42, got %d", *SparseSet_get(&set, 5, int));
    TEST(set.numElements == 16, "numElements expected 16 after inserts, got %zu", set.numElements);

    // Exercise keep-order remove on a safe path (remove last element).
    SparseSet_remove_keepOrder(&set, 15);
    TEST(set.numElements == 15, "numElements expected 15 after keepOrder remove(last), got %zu", set.numElements);
    TEST(set.data.size == 15, "data.size expected 15 after keepOrder remove(last), got %zu", set.data.size);
    TEST(*SparseSet_get(&set, 0, int) == 12, "id 0 should remain 12 after removing last, got %d", *SparseSet_get(&set, 0, int));
    TEST(*SparseSet_get(&set, 5, int) == 42, "id 5 should remain 42 after removing last, got %d", *SparseSet_get(&set, 5, int));

    typedef struct {
        int x;
        int y;
    } Pair;

    SparseSet pairSet = SparseSet_new(Pair, 4);
    size_t p0 = SparseSet_add(&pairSet, ((Pair){.x = 1, .y = 2}));
    size_t p1 = SparseSet_add(&pairSet, ((Pair){.x = -3, .y = 9}));
    TEST(p0 == 0, "pairSet first id expected 0, got %zu", p0);
    TEST(p1 == 1, "pairSet second id expected 1, got %zu", p1);

    Pair* first = SparseSet_get(&pairSet, 0, Pair);
    Pair* second = SparseSet_get(&pairSet, 1, Pair);
    TEST(first->x == 1 && first->y == 2, "pairSet first value mismatch, got {%d,%d}", first->x, first->y);
    TEST(second->x == -3 && second->y == 9, "pairSet second value mismatch, got {%d,%d}", second->x, second->y);

    SparseSet_remove_keepOrder(&pairSet, 1);
    TEST(pairSet.numElements == 1, "pairSet numElements expected 1 after keepOrder remove(last), got %zu", pairSet.numElements);
    TEST(pairSet.data.size == 1, "pairSet data.size expected 1 after keepOrder remove(last), got %zu", pairSet.data.size);
    Pair* remaining = SparseSet_get(&pairSet, 0, Pair);
    TEST(remaining->x == 1 && remaining->y == 2, "pairSet remaining value mismatch, got {%d,%d}", remaining->x, remaining->y);

    SparseSet_delete(&set);
    SparseSet_delete(&pairSet);

    Test_sparseSet_removeKeepOrder();
    Test_sparseSet_edgeCases();
}

static void Test_dataStructures() {
    Test_arrayList();
    INFO_("Array passed");
    Test_hashMap();
    INFO_("Hashmap passed");
    Test_sparseSet();
    INFO_("SparseSet passed");
}

void Test_run() {
    Test_dataStructures();
}

