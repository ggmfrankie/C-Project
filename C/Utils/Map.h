//
// Created by Stefan on 17.10.2025.
//

#ifndef C_MAP_H
#define C_MAP_H
#include <stdbool.h>
#include <stdlib.h>

#define MAP(name, Key, Value)\
\
typedef struct MapEntry_##name {\
    Key key;\
    Value value;\
} MapEntry_##name;\
\
typedef struct Map_##name {\
    MapEntry_##name* content;\
    bool (*equals)(const Key*, const Key*);\
    size_t capacity;\
    size_t size;\
    void (*put)(struct Map_##name *map, const Key key, const Value value);\
    Value (*get)(const struct  Map_##name *map, const Key key);\
    void (*delete)(struct Map_##name *map);\
} Map_##name;\
\
static inline void name##_Map_put(Map_##name *map, const Key key, const Value value) {\
    if (map->size == map->capacity){\
        MapEntry_##name *temp = realloc(map->content, sizeof(MapEntry_##name) * map->capacity*2);\
        if(temp) map->content = temp;\
    }\
    map->content[map->size] = (MapEntry_##name){\
        .key = key,\
        .value = value\
    };\
    map->size++;\
    map->capacity *= 2;\
}\
\
static inline Value name##_Map_get(const Map_##name *map, const Key key) {\
    for (size_t i = 0; i < map->size; i++) {\
            if (map->equals(&map->content[i].key, &key)) return map->content[i].value;\
    }\
    return (Value){};\
}\
\
static inline void name##_Map_delete(Map_##name *map) {\
    free(map->content);\
    map->content = NULL;\
    map->size = 0;\
    map->capacity = 0;\
}\
static inline Map_##name newMap_##name(const size_t capacity, bool (*equals)(const Key*, const Key*)) {\
    return (Map_##name){\
        .content = malloc(sizeof(MapEntry_##name) * capacity),\
        .capacity = capacity,\
        .size = 0,\
        .equals = equals,\
        .put = name##_Map_put,\
        .get = name##_Map_get,\
        .delete = name##_Map_delete\
    };\
}\

#endif //C_MAP_H