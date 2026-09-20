//
// Created by ertls on 17.09.2026.
//

#pragma once
#include "Utils/Typedef.h"
#include "Utils/DataStructures/CStr.h"
#include "Utils/Macros/Defer.h"
#include "../DataStructures/CString.h"

typedef enum {
    CJSON_NULL,
    CJSON_NUMBER,
    CJSON_STRING,
    CJSON_BOOLEAN,
    CJSON_OBJECT,
    CJSON_ARRAY
} CJsonType;

typedef struct CJson {
    CJsonType type;
    union {
        bool boolValue;
        double numberValue;
        String stringValue;

        struct CJson* aArrayMembers;
        struct CJsonMember* aObjectMembers;
    };
} CJson;

typedef struct CJsonMember {
    String key;
    CJson value;
} CJsonMember;

typedef enum {
    CJSON_KEY,
    CJSON_IDX
} CJsonKeyType;

#define CJsonKey(x) (_CJsonKey){ .type = CJSON_KEY, .key = (x)}
#define CJsonIdx(x) (_CJsonKey){ .type = CJSON_IDX, .index = (x)}

typedef struct {
    CJsonKeyType type;
    union {
        char* key;
        int index;
    };
} _CJsonKey;

CJson CJson_parse(const char* jsonString);

CJson* _CJson_get(const CJson* json, _CJsonKey keys[], int keyLen);
#define CJson_get(json, ...) _CJson_get(json, (_CJsonKey[]){__VA_ARGS__}, sizeof((_CJsonKey[]){__VA_ARGS__})/sizeof(_CJsonKey))

CJson* _CJson_get2(const CJson* json, int len, ...);
#define CJson_get2(json, ...) _CJson_get2(json, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

CJson CJson_newString(const char* value);
CJson CJson_newNumber(double value);
CJson CJson_newBool(bool value);
CJson CJson_newNull(const void*);

CJson _CJson_identity(CJson value);

#define CJson_value(value) _Generic((value),\
    int: CJson_newNumber,\
    float: CJson_newNumber,\
    double: CJson_newNumber,\
    const char*: CJson_newString,\
    char*: CJson_newString,\
    bool: CJson_newBool,\
    CJson: _CJson_identity,\
    void*: CJson_newNull\
)(value)

CJsonMember CJson_newMember(const char* key, CJson value);
#define CJson_member(key, value) CJson_newMember((key), CJson_value(value))

void CJson_appendNewMember(CJson* json, const char* key, CJson value);
#define CJson_appendMember(json, key, value) CJson_appendNewMember(json, key, CJson_value(value))

CJson _CJson_newObject(CJsonMember member[], size_t numMembers);
#define CJson_object(...) _CJson_newObject((CJsonMember[]){__VA_ARGS__}, sizeof((CJsonMember[]){__VA_ARGS__})/sizeof(CJsonMember))

CJson _CJson_newArray(CJson member[], size_t numMembers);
#define CJson_array(...) _CJson_newArray((CJson[]){__VA_ARGS__}, sizeof((CJson[]){__VA_ARGS__})/sizeof(CJson))

void CJson_free(CJson* json);

Str CJson_dump(const CJson* json);
void CJson_print(const CJson* json);
