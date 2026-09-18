//
// Created by ertls on 17.09.2026.
//

#pragma once
#include "Utils/Typedef.h"
#include "Utils/DataStructures/CString.h"
#include "Utils/Macros/Defer.h"
#include "Utils/_NYI_/String.h"

typedef enum : byte {
    CJSON_NULL,
    CJSON_NUMBER,
    CJSON_STRING,
    CJSON_BOOLEAN,
    CJSON_OBJECT,
    CJSON_ARRAY
} CJsonType;

typedef struct CJson {
    CJsonType type;
    char _unused[7];
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

CJson CJson_parse(const char* jsonString);

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

static void CJson_test() {
    int a = 342;

    defer(CJson_free) auto json = CJson_object(
        CJson_member("test", 12012),
        CJson_member("test2", "12012"),
        CJson_member("boolean sderearaerarar", true),
        CJson_member("test4",
            CJson_array(
                CJson_value("affe"),
                CJson_object(
                    CJson_member("sfda", a),
                    CJson_member("twet", NULL)
                )
            )
        )
    );

    CJson_print(&json);

    auto json2 = CJson_parse("{\"test\":12012.000000,\"test2\":\"12012\",\"boolean\":true}");

    CJson_print(&json2);
}