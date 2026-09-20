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
} CJson_ParserState;

static const char* CJson_ParserStateNames[] ={
    "PARSER_SEARCH",
    "PARSER_NUMBER",
    "PARSER_STRING",
};

static CJson CJson_parseObject(int* i, const char* jsonString, int len, Str* builder);

static CJson CJson_parseArray(int* i, const char* jsonString, int len, Str* builder) {
    CJson_ParserState state = PARSER_SEARCH;

    CJson object = CJson_array(
        (CJson){}
    );

    CJson* currMember = arrPeek(object.aArrayMembers);

    bool foundDecimal = false;
    bool isEscaped = false;

    if (jsonString[(*i)++] != '[') goto Error;

    for (;*i < len; ++*i) {
       char c = jsonString[*i];

        switch (state) {
            PARSER_SEARCH:
            c = jsonString[*i];
            case PARSER_SEARCH: {
                if (isblank(c)) break;

                if (c == ']') {
                    if (!arrIsEmpty(object.aObjectMembers)) arrPop(object.aObjectMembers);
                    return object;
                }

                if (c == ',') {break;}

                if (c == '"')                       goto PARSER_STRING;
                if (isdigit(c)||c == '+'||c == '-') goto PARSER_NUMBER;
                if (c == 't'||c == 'f')             goto PARSER_BOOL;
                if (c == 'n')                       goto PARSER_NULL;
                if (c == '{')                       goto PARSER_OBJECT;
                if (c == '[')                       goto PARSER_ARRAY;;

                goto Error;
            } break;

            PARSER_NUMBER: {
                state = PARSER_NUMBER;
                strAppend(builder, c);
            } break;
            case PARSER_NUMBER: {
                if (isdigit(c) || (c == '.' && !foundDecimal)) {
                    strAppend(builder, c);
                    if (c == '.') foundDecimal = true;
                } else if (isblank(c) || c == ',') {
                    foundDecimal = false;
                    *currMember = CJson_newNumber(strtod(*builder, nullptr));
                    goto PushNewMember;
                } else goto Error;
            } break;

            PARSER_STRING: {
                assert(c == '"');
                state = PARSER_STRING;
            } break;
            case PARSER_STRING: {
                if (c == '\\') isEscaped = true;
                else if (isEscaped || c!= '"') strAppend(builder, c);
                else {
                    *currMember = CJson_newString(*builder);
                    ++*i;
                    goto PushNewMember;
                }
            } break;

            PARSER_BOOL: {
                assert(c == 't' || c == 'f');
                const int isValid = (c == 't') ?
                    (memcmp(&jsonString[*i], "true", 4)==0) *4:
                    (memcmp(&jsonString[*i], "false", 5)==0)*5;

                Log_trace("IsValid: %i\n", isValid);
                if (!isValid)
                    goto Error;
                *i += isValid;
                *currMember = CJson_newBool(isValid==4);
                goto PushNewMember;
            } break;

            PARSER_NULL: {
                const bool isValid = memcmp(&jsonString[*i], "null", 4)==0;
                if (!isValid) goto Error;
                *i += 4;
                *currMember = CJson_newNull(NULL);
                goto PushNewMember;
            } break;

            PARSER_OBJECT: {
                *currMember = CJson_parseObject(i, jsonString, len, builder);
                ++*i;
                goto PushNewMember;
            } break;

            PARSER_ARRAY: {
                *currMember = CJson_parseArray(i, jsonString, len, builder);
                ++*i;
                goto PushNewMember;
            } break;
        }
        Log_trace("Builder: %s ""State: %s ""Current char: %c", builder, CJson_ParserStateNames[state], c);
        continue;

        PushNewMember:
        state = PARSER_SEARCH;
        strClear(*builder);
        arrPush(object.aArrayMembers, (CJson){});
        currMember = arrPeek(object.aArrayMembers);
        goto PARSER_SEARCH;

        Error:
        ERROR_("Invalid argument: %c\n State: %s", jsonString[*i], CJson_ParserStateNames[state]);
        return (CJson){};
    }

    return object;
}

static CJson CJson_parseObject(int* i, const char* jsonString, int len, Str* builder) {
    CJson_ParserState state = PARSER_SEARCH;

    CJson object = CJson_object(
        (CJsonMember){}
    );

    CJsonMember* currMember = arrPeek(object.aObjectMembers);

    bool isKey = true;
    bool foundDecimal = false;
    bool isEscaped = false;

    if (jsonString[(*i)++] != '{') goto Error;

    for (;*i < len; ++*i) {
       char c = jsonString[*i];

        switch (state) {
            PARSER_SEARCH:
            c = jsonString[*i];
            case PARSER_SEARCH: {
                if (isblank(c)) break;

                if (c == '}') {
                    if (!arrIsEmpty(object.aObjectMembers)) arrPop(object.aObjectMembers);
                    return object;
                }

                if (c == ':') {isKey = false; break;}
                if (c == ',') {isKey = true;  break;}

                if (c == '"')                       goto PARSER_STRING;
                if (isKey)                          goto Error;
                if (isdigit(c)||c == '+'||c == '-') goto PARSER_NUMBER;
                if (c == 't'||c == 'f')             goto PARSER_BOOL;
                if (c == 'n')                       goto PARSER_NULL;
                if (c == '{')                       goto PARSER_OBJECT;
                if (c == '[')                       goto PARSER_ARRAY;;

                goto Error;
            } break;

            PARSER_NUMBER: {
                state = PARSER_NUMBER;
                strAppend(builder, c);
            } break;
            case PARSER_NUMBER: {
                if (isdigit(c) || (c == '.' && !foundDecimal)) {
                    strAppend(builder, c);
                    if (c == '.') foundDecimal = true;
                } else if (isblank(c) || c == ',') {
                    foundDecimal = false;
                    const double num = strtod(*builder, nullptr);
                    currMember->value = CJson_newNumber(num);
                    goto PushNewMember;
                } else goto Error;
            } break;

            PARSER_STRING: {
                assert(c == '"');
                state = PARSER_STRING;
            } break;
            case PARSER_STRING: {
                if (c == '\\') isEscaped = true;
                else if (isEscaped || c!= '"') strAppend(builder, c);
                else {
                    if (isKey) {
                        state = PARSER_SEARCH;
                        currMember->key = String_new(*builder);
                        strClear(*builder);
                    }
                    else {
                        currMember->value = CJson_newString(*builder);
                        ++*i;
                        goto PushNewMember;
                    }
                }
            } break;

            PARSER_BOOL: {
                assert(c == 't' || c == 'f');
                const int isValid = (c == 't') ?
                    (memcmp(&jsonString[*i], "true", 4)==0) *4:
                    (memcmp(&jsonString[*i], "false", 5)==0)*5;

                Log_trace("IsValid: %i\n", isValid);
                if (!isValid)
                    goto Error;
                *i += isValid;
                currMember->value = CJson_newBool(isValid==4);
                goto PushNewMember;
            } break;

            PARSER_NULL: {
                const bool isValid = memcmp(&jsonString[*i], "null", 4)==0;
                if (!isValid) goto Error;
                *i += 4;
                currMember->value = CJson_newNull(NULL);
                goto PushNewMember;
            } break;

            PARSER_OBJECT: {
                currMember->value = CJson_parseObject(i, jsonString, len, builder);
                ++*i;
                goto PushNewMember;
            } break;

            PARSER_ARRAY: {
                currMember->value = CJson_parseArray(i, jsonString, len, builder);
                ++*i;
                goto PushNewMember;
            } break;
        }
        Log_trace("Builder: %s ""State: %s ""Current char: %c", builder, CJson_ParserStateNames[state], c);
        continue;

        PushNewMember:
        state = PARSER_SEARCH;
        strClear(*builder);
        arrPush(object.aObjectMembers, (CJsonMember){});
        currMember = arrPeek(object.aObjectMembers);
        goto PARSER_SEARCH;

        Error:
        ERROR_("Invalid argument: %c\n State: %s", jsonString[*i], CJson_ParserStateNames[state]);
        return (CJson){};
    }

    return object;
}

CJson CJson_parse(const char* jsonString) {
    const size_t len = strlen(jsonString);
    defer(strFree) Str builder = strNew(32);
    int i = 0;
    return CJson_parseObject(&i, jsonString, len, &builder);
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
    if (json->type != CJSON_OBJECT) ERROR_("Cannot add member to a non object type");
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
            strAppend_sprintf(buffer, "%s", String_getValue(value->stringValue));
            break;
        case CJSON_NUMBER:
            strAppend_sprintf(buffer, "%f", value->numberValue);
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
