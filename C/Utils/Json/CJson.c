//
// Created by ertls on 17.09.2026.
//

#include "CJson.h"

#include <string.h>

#include "Utils/DataStructures/CArrayList.h"
#include "Utils/Macros/Defer.h"
#include <ctype.h>

typedef enum {
    PARSER_SEARCH,
    PARSER_NUMBER,
    PARSER_STRING,
    PARSER_BOOL,
    PARSER_NULL,
    PARSER_OBJECT,
    PARSER_ARRAY,
} CJson_ParserState;

CJson CJson_parseArray(int* i, const char* jsonString, int len) {
    TODO_("implement");
}

static CJson CJson_parseObject(int* i, const char* jsonString, int len) {
    CJson_ParserState state = PARSER_SEARCH;
    defer(strFree) Str builder = strNew(32);

    CJson object = CJson_object(
        (CJsonMember){}
    );

    CJsonMember* currMember = arrPeek(object.aObjectMembers);

    bool isKey = false;
    bool foundDecimal = false;
    bool isEscaped = false;

    if (jsonString[(*i)++] != '{') goto Error;

    for (;*i < len; ++*i) {
        const char c = jsonString[*i];

        switch (state) {
            case PARSER_SEARCH: {
                if (isblank(c)) break;

                if (c == '}') return object;

                if (c == ':') {isKey = false; break;}
                if (c == ',') {isKey = true; break;}
                if (c == '"') {state = PARSER_STRING; break;}
                if (isKey) goto Error;
                else if (isdigit(c)||c == '+'||c == '-') state = PARSER_NUMBER;
                else if (c == 't'||c == 'f') state = PARSER_BOOL;
                else if (c == 'n') state = PARSER_NULL;

                else goto Error;
                strAppend(&builder, c);
            } break;

            case PARSER_NUMBER: {
                if (isdigit(c) || (c == '.' && !foundDecimal)) {
                    strAppend(&builder, c);
                    foundDecimal = true;
                } else if (isblank(c) || c == ',') {
                    foundDecimal = false;
                    state = PARSER_SEARCH;
                    currMember->value = CJson_newNumber(strtod(builder, nullptr));
                } else goto Error;
            } break;

            case PARSER_STRING: {
                if (c == '\\') isEscaped = true;
                else if (isEscaped || c!= '"') strAppend(&builder, c);
                else {
                    state = PARSER_SEARCH;
                    if (isKey) {
                        currMember->key = String_new(builder);
                        strClear(builder);
                    }
                    else {
                        currMember->value = CJson_newString(builder);
                        goto PushNewMember;
                    }
                }
            } break;

            case PARSER_BOOL: {
                static constexpr int lengths[] = {5, 4};
                const int isValid = (c == 't') ?
                    memcmp(&jsonString[*i], "true", 4):
                    memcmp(&jsonString[*i], "false", 5)*2;

                if (!isValid) goto Error;
                *i += lengths[isValid];
                currMember->value = CJson_newBool(isValid);
                goto PushNewMember;
            } break;

            case PARSER_NULL: {
                const bool isValid = memcmp(&jsonString[*i], "null", 4);
                if (!isValid) goto Error;
                *i += 4;
                currMember->value = CJson_newNull(NULL);
                goto PushNewMember;
            } break;

            case PARSER_OBJECT: {
                currMember->value = CJson_parseObject(i, jsonString, len);
                goto PushNewMember;
            } break;

            case PARSER_ARRAY: {
                currMember->value = CJson_parseArray(i, jsonString, len);
                goto PushNewMember;
            } break;
        }

        continue;

        PushNewMember:
        puts(builder);
        strClear(builder);
        arrPush(object.aObjectMembers, (CJsonMember){});
        currMember = arrPeek(object.aObjectMembers);
    }
    Error:
    ERROR_("Invalid argument");
    return (CJson){};
}

CJson CJson_parse(const char* jsonString) {
    const size_t len = strlen(jsonString);
    int i = 0;
    return CJson_parseObject(&i, jsonString, len);
}

CJson CJson_newString(const char* value) {
    return (CJson) {
        .type = CJSON_STRING,
        .stringValue = String_new(value)
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

CJson CJson_newNull(const void*) {
    return (CJson) {
        .type = CJSON_NULL,
        .stringValue = String_new("null")
    };
}

CJson _CJson_identity(CJson value) {
    return value;
}

CJsonMember CJson_newMember(const char* key, CJson value) {
    return (CJsonMember){
        .key = String_new(key),
        .value = value
    };
}

void CJson_appendNewMember(CJson* json, const char* key, CJson value) {
    if (json->type != CJSON_OBJECT) ERROR_("Cannot add member to no object type");
    arrPush(json->aObjectMembers, CJson_member(key, value));
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

void CJson_free(CJson* json);

static void CJson_freeMember(CJsonMember* member) {
    String_free(&member->key);
    CJson_free(&member->value);
}

void CJson_free(CJson* json) {
    switch (json->type) {
        case CJSON_NULL:
        case CJSON_NUMBER:
        case CJSON_BOOLEAN:
            break;
        case CJSON_STRING:
            String_free(&json->stringValue);
            break;
        case CJSON_OBJECT:
            for arrEach(member, json->aObjectMembers) CJson_freeMember(member);
            break;
        case CJSON_ARRAY:
            for arrEach(value, json->aArrayMembers) CJson_free(value);
            break;
    }
}

static void CJson_dumpValue(Str* buffer, const CJson* value);

static void CJson_dumpMember(Str* buffer, const CJsonMember* member) {
    strAppend_sprintf(buffer, "\"%s\"", String_getValue(member->key));
    strAppend(buffer, ':');
    CJson_dumpValue(buffer, &member->value);
    strAppend(buffer, ',');
}

static void CJson_dumpValue(Str* buffer, const CJson* value) {
    switch (value->type) {
        case CJSON_NULL:
            strAppend_sprintf(buffer, String_getValue(value->stringValue));
            break;
        case CJSON_NUMBER:
            strAppend_sprintf(buffer, "%llf", value->numberValue);
            break;
        case CJSON_STRING:
            strAppend_sprintf(buffer, "\"%s\"", String_getValue(value->stringValue));
            break;
        case CJSON_BOOLEAN:
            strAppend_sprintf(buffer, value->boolValue ? "true":"false");
            break;
        case CJSON_OBJECT:
            strAppend(buffer, '{');
            for arrEach(member, value->aObjectMembers) CJson_dumpMember(buffer, member);
            if (!arrIsEmpty(value->aObjectMembers))  strPop(buffer);
            strAppend(buffer, '}');
            break;
        case CJSON_ARRAY:
            strAppend(buffer, '[');
            for arrEach(member, value->aArrayMembers) {CJson_dumpValue(buffer, member); strAppend(buffer, ',');}
            if (!arrIsEmpty(value->aArrayMembers)) strPop(buffer);
            strAppend(buffer, ']');
            break;
    }
}

Str CJson_dump(const CJson* json) {
    assert(json != nullptr);
    Str out = strNew(64);
    CJson_dumpValue(&out, json);
    strFit(&out);
    return out;
}

void CJson_print(const CJson* json) {
    defer(defer_strDelete) Str dump = CJson_dump(json);
    puts(dump);
}
