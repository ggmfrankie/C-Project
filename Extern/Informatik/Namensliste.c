//
// Created by ertls on 27.11.2025.
//

#include "Namensliste.h"
#include <string.h>
#include <windows.h>
#include <_bsd_types.h>

#include "../../Utils/String.h"
#include "../../Utils/FileIO.h"
#include "../../Utils/HashMap.h"
#include "../../Utils/TimeMeasurenments.h"

#define MAX_ELEMENTS 500
#define MAX_NAME 15100

//#define LANGSAM

HASH_MAP(NameCounter, char*, int)

typedef struct {
#ifdef LANGSAM
    char name[MAX_NAME];
#else
    char *name;
#endif
    int count;
} NameCounter;

typedef struct {
    NameCounter *content;
    size_t size;
    size_t capacity;
} NameCounterMap;

ssize_t getIndex(const NameCounterMap *ncm, const char *key);
void addOrIncrement(NameCounterMap *ncm, char *key);
void printNamesWithCount(const NameCounterMap *ncm, size_t start, size_t end);
void namensliste_langsam();
void bubbleSort(const NameCounterMap* ncm);
void enterName(const NameCounterMap *ncm);
void namensliste_schnell();


void namensliste_Aufgabe(void* userdata) {
    namensliste_langsam();
    namensliste_schnell();
}

void namensliste_langsam() {
    const String path = stringOf("../Extern/Informatik/Data/vornamen.txt");
    String nameList = readFile(&path);
    List_String names = Strings.split(&nameList, "\n");
    names.size--;

    Strings.delete(&nameList);


    NameCounterMap nameCounterMap = (NameCounterMap){
        .content =  (NameCounter*)calloc(MAX_ELEMENTS, sizeof(NameCounter)),
        .size = 0,
        .capacity = MAX_ELEMENTS
    };


    const u_int64 startNs = now_ns();
    forEach(names, const String, name,
        addOrIncrement(&nameCounterMap, name.content);
    )
    bubbleSort(&nameCounterMap);
    const u_int64 time = now_ns() - startNs;

    printNamesWithCount(&nameCounterMap, 0 , nameCounterMap.size);

    printf("Time elapsed: %llu ns\n", time);

    enterName(&nameCounterMap);

    free(nameCounterMap.content);
    String_ListFree(&names);
}

void addOrIncrement(NameCounterMap *ncm, char *key) {
    const ssize_t index = getIndex(ncm, key);
    if (index >= 0) ncm->content[index].count++;
    else {
    #ifdef LANGSAM
        strcpy(ncm->content[ncm->size].name,key);
    #else
        ncm->content[ncm->size].name = key;
    #endif

        ncm->content[ncm->size++].count = 1;
    }
}

ssize_t getIndex(const NameCounterMap *ncm, const char *key) {
    for (int i = 0; i < ncm->size; i++) {
        if (!strcmp(ncm->content[i].name, key)) {
            return i;
        }
    }
    return -1;
}

void printNamesWithCount(const NameCounterMap *ncm, const size_t start, const size_t end) {
    if (end > ncm->size) puts("indices out of Bounds");
    for (size_t i = start; i < end; i++) {
        printf("%llu. %s (%ix)\n",i ,ncm->content[i].name, ncm->content[i].count);
    }
}

void bubbleSort(const NameCounterMap* ncm) {
    bool needsSorting = true;
    while (needsSorting) {
        needsSorting = false;
        NameCounter* nameCounters = ncm->content;
        for (int i = 0 ; i < ncm->size-1; i++) {
            if (nameCounters[i].count < nameCounters[i+1].count) {
                const NameCounter temp = nameCounters[i];
                nameCounters[i] = nameCounters[i+1];
                nameCounters[i+1] = temp;
                needsSorting = true;
            }
        }
    }
}

void enterName(const NameCounterMap *ncm) {
    char name[512];
    ssize_t index = -1;

    AGAIN:
    puts("Geben Sie ihren Namen ein: ");
    scanf("%511s", name);
    puts("-----------------------------");
    index = getIndex(ncm, name);
    if (index == -1) {
        puts("Name nicht in der Liste :-(");
        return;
    }
    printf("%s steht an %llu. Stelle und ist %ix vorhanden:\n\n", name, index, ncm->content[index].count);

    printNamesWithCount(ncm, max(index-2, 0), index+1);
    goto AGAIN;
}

void namensliste_schnell() {
    const String path = stringOf("../Extern/Informatik/Data/vornamen.txt");
    const String nameList = readFile(&path);
    List_String names = Strings.split(&nameList, "\n");
    Hashmap_NameCounter nameCounter = newHashmap_NameCounter(512);
    names.size--;

    const u_int64 startNs = now_ns();
    forEach(names, const String, content,
        int* count = Hashmap_NameCounter_get(&nameCounter, content.content);
        if (count) {
            (*count)++;
        } else {
            Hashmap_NameCounter_add(&nameCounter, content.content, 1);
        }
    )
    const u_int64 time = now_ns() - startNs;

    for (int i = 0; i < nameCounter.capacity; i++) {
        if (nameCounter.content[i].isOccupied) {
            printf("%s = %i\n", nameCounter.content[i].key, nameCounter.content[i].value);
        }
    }

    printf("Time elapsed: %llu ns", time);
}