//
// Created by Stefan on 14.05.2026.
//

#pragma once
#include <assert.h>
#include <stdio.h>
#include "Utils/Typedef.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    size_t size;
    size_t capacity;
} _HashMap_Header_;

#define _HashMapInitCapacity 256
#define _HashMapSeed 342341431UL
#define _HashMapContentSize(map) (sizeof(char*) + sizeof(*(map)))

#define _hashMapGetHead(map) (&((_HashMap_Header_*)(map))[-1])
#define _hashMapKey(_place) (*(const char**)(_place))
#define _hashMapValue(map, _place) (*(typeof(*(map))*)((byte*)(_place) + sizeof(char*)))

#define mapLen(map) ((map) ? (_hashMapGetHead(map)->size) : 0)
#define mapCap(map) ((map) ? (_hashMapGetHead(map)->capacity) : 0)

#define mapEmpty(map) (arrLen(map) == 0)

uint32_t _hashMapHash(const char* key);

#define mapInsert(map, key, value...)\
    do {\
        if ((map) == nullptr) {\
            _HashMap_Header_* _header = calloc(_HashMapContentSize(map) * _HashMapInitCapacity + sizeof(_HashMap_Header_), 1);\
            assert(_header != nullptr);\
            _header->capacity = _HashMapInitCapacity;\
            _header->size = 0;\
            (map) = (void*) (_header+1);\
        }\
        _HashMap_Header_* _header = _hashMapGetHead(map);\
        if (_header->size / (float)_header->capacity > 0.75f) /* GROW */{\
            assert(_header->capacity != 0);\
            const size_t _newCapacity = _header->capacity * 2;\
            \
            _HashMap_Header_* _newHeader = calloc(_HashMapContentSize(map) * _newCapacity + sizeof(_HashMap_Header_), 1);\
            byte* _newContet = (byte*)(_newHeader+1);\
            \
            assert(_newContet!= nullptr);\
            int _i = 0;\
            for (byte* _curr = (byte*) map; _i < _header->capacity; _curr += _HashMapContentSize(map), ++_i) {\
                const char* _key_ = _hashMapKey(_curr);\
                if (_key_ == nullptr) continue;\
                \
                byte* newPlace = _newContet + _hashMapHash(_key_) % _newCapacity * _HashMapContentSize(map);\
            \
                const byte* _end = _newContet + _HashMapContentSize(map) * _newCapacity;\
                while (_hashMapKey(newPlace) != nullptr) {\
                    newPlace += _HashMapContentSize(map);\
                    if (newPlace >= _end) {\
                        newPlace = _newContet;\
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
        byte* _place = ((byte*) map) + _HashMapContentSize(map) * (_hashMapHash(key) % _header->capacity);\
        const byte* _end = ((byte*) map) + _HashMapContentSize(map) * _header->capacity;\
        while (_hashMapKey(_place) != nullptr) {\
            _place += _HashMapContentSize(map);\
            if (_place >= _end) {\
                _place = ((byte*) map);\
            }\
        }\
        _hashMapKey(_place) = key;\
        _hashMapValue(map, _place) = value;\
        ++_header->size;\
    } while (0)

void* _hashMapGet(byte* map, const char* key, size_t stride);
#define mapGet(map, key) (typeof(*map)*) _hashMapGet((byte*)(map), (key), _HashMapContentSize(map))

void _hashMapDelete(void* map);
#define mapDelete(map) _hashMapDelete(map)
