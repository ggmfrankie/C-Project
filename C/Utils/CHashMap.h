//
// Created by Stefan on 14.05.2026.
//

#pragma once
#include <assert.h>
#include <stdio.h>
#include "Utils/UtilsTypedef.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
} _HashMap_Header_;

#define _HashMapInitCapacity_ 256
#define _HashMapSeed_ 342341431UL
#define _HashMapContentSize_(map) (sizeof(char*) + sizeof(*(map)))

#define _hashMapGetHead_(map) (((_HashMap_Header_*)(map))[-1])
#define _hashMapKey(_place) (*(const char**)(_place))
#define _hashMapValue(map, _place) (*(typeof(*(map))*)((byte*)(_place) + sizeof(char*)))

#define mapLen(map) ((map) ? (_hashMapGetHead_(map).size) : 0)
#define mapCap(map) ((map) ? (_hashMapGetHead_(map).capacity) : 0)

#define mapEmpty(map) (arrLen(map) == 0)

uint32_t _hashMapHash_(const char* key);

#define mapInsert(map, key, value...)\
    do {\
        if ((map) == nullptr) {\
            _HashMap_Header_* _header = calloc(_HashMapContentSize_(map) * _HashMapInitCapacity_ + sizeof(_HashMap_Header_), 1);\
            assert(_header != nullptr);\
            _header->capacity = _HashMapInitCapacity_;\
            _header->size = 0;\
            (map) = (void*) (_header+1);\
        }\
        _HashMap_Header_* _header = &_hashMapGetHead_(map);\
        if (_header->size / (float)_header->capacity > 0.75f) /* GROW */{\
            assert(_header->capacity != 0);\
            const size_t _newCapacity = _header->capacity * 2;\
            \
            _HashMap_Header_* _newHeader = calloc(_HashMapContentSize_(map) * _newCapacity + sizeof(_HashMap_Header_), 1);\
            byte* newContent = (byte*)(_newHeader+1);\
            \
            assert(newContent!= nullptr);\
            int _i = 0;\
            for (byte* _curr = (byte*) map; _i < _header->capacity; _curr += _HashMapContentSize_(map), ++_i) {\
                const char* _key_ = _hashMapKey(_curr);\
                if (_key_ == nullptr) continue;\
                \
                byte* newPlace = newContent + _hashMapHash_(_key_) % _newCapacity * _HashMapContentSize_(map);\
            \
                const byte* _end = newContent + _HashMapContentSize_(map) * _newCapacity;\
                while (_hashMapKey(newPlace) != nullptr) {\
                    newPlace += _HashMapContentSize_(map);\
                    if (newPlace >= _end) {\
                        newPlace = newContent;\
                    }\
                }\
            \
                _hashMapKey(newPlace) = _hashMapKey(_curr);\
                _hashMapValue(map, newPlace) = _hashMapValue(map, _curr);\
            }\
            _newHeader->size = _header->size;\
            _newHeader->capacity = _newCapacity;\
            free(_header);\
            _header = _newHeader;\
            (map) = (void*) (_header+1);\
        }\
        byte* _place = ((byte*) map) + _HashMapContentSize_(map) * (_hashMapHash_(key) % _header->capacity);\
        const byte* _end = ((byte*) map) + _HashMapContentSize_(map) * _header->capacity;\
        while (_hashMapKey(_place) != nullptr) {\
            _place += _HashMapContentSize_(map);\
            if (_place >= _end) {\
                _place = ((byte*) map);\
            }\
        }\
        _hashMapKey(_place) = key;\
        _hashMapValue(map, _place) = value;\
        ++_header->size;\
    } while (0)

void* _hashMapGet_(void* map, const char* key, size_t stride);
#define mapGet(map, key) (typeof(*map)*) _hashMapGet_((map), (key), _HashMapContentSize_(map))


/*
auto p = &_hashMapValue(hTest, _place);
        size_t idx = ((byte*)_place - (byte*)hTest) / _HashMapContentSize_(hTest);
        printf("7 placed in %p,\n which is index %llu\n", p , idx);
*/