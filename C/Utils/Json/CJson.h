//
// Created by ertls on 17.09.2026.
//

#pragma once
#include "Utils/DataStructures/CString.h"

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
        Str stringValue;

        struct CJson* aArrayMembers;
        struct CJsonMember* aObjectMembers;
    };
} CJson;

typedef struct CJsonMember {
    char* key;
    CJson value;
} CJsonMember;

CJson CJson_newString(const char* value);
CJson CJson_newNumber(double value);
CJson CJson_newBool(bool value);

CJson _CJson_identity(CJson value);

#define CJson_new(value) _Generic((value),\
    int: CJson_newNumber,\
    float: CJson_newNumber,\
    double: CJson_newNumber,\
    const char*: CJson_newString,\
    char*: CJson_newString,\
    bool: CJson_newBool,\
    CJson: _CJson_identity\
)(value)

CJsonMember CJson_newMember(const char* key, CJson value);
#define CJson_member(key, value) CJson_newMember((key), CJson_new(value))

CJson _CJson_newObject(CJsonMember member[], size_t numMembers);
#define CJson_object(...) _CJson_newObject((CJsonMember[]){__VA_ARGS__}, sizeof((CJsonMember[]){__VA_ARGS__})/sizeof(CJsonMember))

CJson _CJson_newArray(CJson member[], size_t numMembers);
#define CJson_array(...) _CJson_newArray((CJson[]){__VA_ARGS__}, sizeof((CJson[]){__VA_ARGS__})/sizeof(CJson))

void CJson_print(const CJson* json);

static void CJson_test() {

    auto json = CJson_object(
        CJson_member("test", 12012),
        CJson_member("test2", "12012"),
        CJson_member("test4",
            CJson_array(
                CJson_new("affe"),
                CJson_object(
                    CJson_member("sfda", 6556)
                )
            )
        )
    );

    CJson_print(&json);
}