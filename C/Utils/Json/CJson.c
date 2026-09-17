//
// Created by ertls on 17.09.2026.
//

#include "CJson.h"

#include "Utils/DataStructures/CArrayList.h"

CJson CJson_newString(const char* value) {
    return (CJson){
        .type = CJSON_STRING,
        .stringValue = strNew_copy(value)
    };
}

CJson CJson_newNumber(double value) {
    return (CJson){
        .type = CJSON_NUMBER,
        .numberValue = value
    };
}

CJson CJson_newBool(bool value) {
    return (CJson){
        .type = CJSON_BOOLEAN,
        .boolValue = value
    };
}

CJson _CJson_identity(CJson value) {
    return value;
}

CJsonMember CJson_newMember(const char* key, CJson value) {
    return (CJsonMember){
        .key = strNew_copy(key),
        .value = value
    };
}

CJson _CJson_newObject(CJsonMember member[], size_t numMembers) {
    CJson out = {.type = CJSON_OBJECT};
    for (int i = 0; i < numMembers; ++i) {
        arrPush(out.aObjectMembers, member[i]);
    }
    return out;
}

CJson _CJson_newArray(CJson member[], size_t numMembers) {
    CJson out = {.type = CJSON_ARRAY};
    for (int i = 0; i < numMembers; ++i) {
        arrPush(out.aArrayMembers, member[i]);
    }
    return out;
}

static void CJson_dumpValue(Str* buffer, const CJson* value);

static void CJson_dumpMember(Str* buffer, const CJsonMember* member) {
    strAppend_sprintf(buffer, "\"%s\"", member->key);
    strAppend(buffer, ':');
    CJson_dumpValue(buffer, &member->value);
    strAppend(buffer, ' ');
}

static void CJson_dumpValue(Str* buffer, const CJson* value) {
    switch (value->type) {
        case CJSON_NULL: break;
        case CJSON_NUMBER:
            strAppend_sprintf(buffer, "%llf", value->numberValue);
            break;
        case CJSON_STRING:
            strAppend_sprintf(buffer, "\"%s\"", value->stringValue);
            break;
        case CJSON_BOOLEAN:
            strAppend_sprintf(buffer, value->boolValue ? "true":"false");
            break;
        case CJSON_OBJECT:
            strAppend(buffer, '{');
            for arrEach(member, value->aObjectMembers) CJson_dumpMember(buffer, member);
            strAppend(buffer, '}');
            break;
        case CJSON_ARRAY:
            strAppend(buffer, '[');
            for arrEach(member, value->aArrayMembers) {CJson_dumpValue(buffer, member); strAppend(buffer, ',');}
            strAppend(buffer, ']');
            break;
    }
}

Str CJson_dump(const CJson* json) {
    assert(json != nullptr);
    Str out = strNew(64);
    CJson_dumpValue(&out, json);
    return out;
}

void CJson_print(const CJson* json) {
    puts(CJson_dump(json));
}
