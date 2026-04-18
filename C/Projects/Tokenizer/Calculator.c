//
// Created by ertls on 12.02.2026.
//

#include "Calculator.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Extern/Libraries/WinAdapt.h"

constexpr int MAX_TOKENS = 64;
constexpr int MAX_TOKEN_LENGTH = 16;

typedef enum : byte{
    s_invalid,
    s_neg,
    s_pos,
    s_operator,
    s_number,
    s_braceOpen,
    s_braceClose,
    s_done
} TokenizerState;

typedef enum : byte{
    t_operator,
    t_num,
    t_decNum,
    t_braceOpen,
    t_braceClose,
} e_TokenType;

typedef struct {
    char content[MAX_TOKEN_LENGTH];
    int length;
    e_TokenType tokenType;
} Token;

typedef bool(*isState)(TokenizerState state);
typedef bool(*isChar)(char c);
typedef void(*ActionFun)(char c);

typedef struct {
    isState prevState;
    isChar event;
    TokenizerState nextState;
    ActionFun actionFun;
} Transition;

typedef struct {
    Token m[MAX_TOKENS];
    int length;
} TokenList;

typedef struct {
    Token* m[MAX_TOKENS];
    int size;
} TokenStack;

typedef enum : byte{
    t_add,
    t_sub,
    t_mul,
    t_div,
    t_pow
} OperatorType;

typedef enum : byte{
    arg_num,
    arg_expr
} ArgumentType;

static void procChar(char c);
static void infixToPostfix(TokenList *list, TokenStack *out);
static double evaluateExpression(const TokenStack* list);

static TokenList tokenList = {};
static TokenizerState tokenizerState = s_braceOpen;

static void printTokens_List(const TokenList* list);
static void printTokens_Stack(const TokenStack* stack);

void Calculator_runParserTest() {
    do {
        tokenizerState = s_braceOpen;
        char input[512] = {};
        tokenList.length = -1;
        memset(tokenList.m, 0, sizeof(tokenList.m));
        puts("Enter an expression:");
#if 1
        scanf("%511s", input);
#else
        strcpy(input, "22-44*(12--4.99)");
#endif
        const auto length = strlen(input);
        for (int i = 0; i <= length; i++ ) {
            procChar(input[i]);
        }
        procChar('\n');
        //printTokens_List(&tokenList);

        TokenStack postfix = {};
        infixToPostfix(&tokenList ,&postfix);
        //printTokens_Stack(&postfix);

        printf("= %lf\n", evaluateExpression(&postfix));
    } while (1);
}

double Calculator_parseMathExpression(const char* input) {
    tokenizerState = s_braceOpen;
    tokenList.length = -1;
    memset(tokenList.m, 0, sizeof(tokenList.m));

    const auto length = strlen(input);
    for (int i = 0; i <= length; i++ ) {
        procChar(input[i]);
    }
    procChar('\n');
    TokenStack postfix = {};
    infixToPostfix(&tokenList ,&postfix);
    return evaluateExpression(&postfix);
}

static double evaluateOperator(OperatorType c, double a, double b) {
    switch (c) {
        case t_add: return (a + b);
        case t_sub: return (a - b);
        case t_mul: return (a * b);
        case t_div: return (a / b);
        case t_pow: return pow(a, b);
        default: return 0;
    }
}

static void push(TokenStack* stack, Token* token) {
    if (stack->size >= MAX_TOKENS) {
        printf("Token stack is to small: stack->size = %i, max = %i\n", stack->size, MAX_TOKENS);
        return;
    }
    stack->m[stack->size++] = token;
}

static Token* pop(TokenStack* stack) {
    if (stack->size <= 0) return nullptr;
    return stack->m[--stack->size];
}

static Token* peek(const TokenStack* stack) {
    if (stack->size <= 0) return nullptr;
    return stack->m[stack->size-1];
}

static bool empty(const TokenStack* stack) {
    return stack->size == 0;
}

static OperatorType getOperatorType(const char* c) {
    if (c == nullptr) { puts("nullptr passed to getOperatorType()"); return 0;}
    const auto length = strlen(c);
    if (length == 1) {
        switch (c[0]) {
            case '+': return t_add;
            case '-': return t_sub;
            case '*': return t_mul;
            case '/': return t_div;
            case '^': return t_pow;
            default: puts("invalid operator"); return 0;
        }
    }
    return 0;
}

static int getOperator(const Token* token) {
    if (token->tokenType != t_operator) { return 0;}
    return getOperatorType(token->content);
}

static void infixToPostfix(TokenList *list, TokenStack *out) {
    TokenStack stack = {};

    for (int i = 0; i < list->length; i++) {
        Token* t = &list->m[i];
        switch (t->tokenType) {
            case t_num:
            case t_decNum:
                push(out, t);
            break;
            case t_braceOpen:
                push(&stack, t);
            break;
            case t_braceClose:
                while (!empty(&stack) && peek(&stack)->tokenType != t_braceOpen) {
                    push(out, pop(&stack));
                }
                pop(&stack);
            break;
            case t_operator:
                while (!empty(&stack) && peek(&stack)->tokenType != t_braceOpen && getOperator(t) <= getOperator(peek(&stack))) {
                    push(out, pop(&stack));
                }
                push(&stack, t);
            break;
        }
    }
    while (!empty(&stack)) push(out, pop(&stack));
}

static double getValue(const Token* token) {
    if (token->tokenType == t_num) return atoi(token->content);
    if (token->tokenType == t_decNum) return atof(token->content);
        puts("Non number token passed to getValue()");
    return 0;
}

#define push(num) stack.m[stack.size++] = num
#define pop() stack.m[--stack.size]
static double evaluateExpression(const TokenStack* list) {
    struct {
        double m[MAX_TOKENS];
        int size;
    } stack;
    stack.size = 0;

    for (int i = 0; i < list->size; i++) {
        const Token* t = list->m[i];
        if (t->tokenType == t_num || t->tokenType == t_decNum) push(getValue(t));
        else if (t->tokenType == t_operator) {
            const double b = pop();
            const double a = pop();
            const double result = evaluateOperator(getOperator(t), a, b);

            push(result);
        }
    }
    return pop();
}
#undef push
#undef pop

static void printTokens_List(const TokenList* list) {
    static char* mapping[] = {"operatorToken", "numberToken", "decNumberToken", "braceOpenToken", "braceCloseToken"};
    for (int i = 0; i < list->length; i++) {
        const auto token = &list->m[i];
        for (int j = 0; j < token->length; j++) {
            printf("%c", token->content[j]);
        }
        printf("\t: %s\n", mapping[token->tokenType]);
    }
}

static void printTokens_Stack(const TokenStack* stack) {
    static char* mapping[] = {"operatorToken", "numberToken", "decNumberToken", "braceOpenToken", "braceCloseToken"};
    for (int i = 0; i < stack->size; i++) {
        const auto token = stack->m[i];
        for (int j = 0; j < token->length; j++) {
            printf("%c", token->content[j]);
        }
        printf("\t: %s\n", mapping[token->tokenType]);
    }
}

static Token* newEmptyToken(e_TokenType type) {
    Token* t = &tokenList.m[++tokenList.length];
    t->tokenType = type;
    return t;
}
static Token* currentToken() {
    return &tokenList.m[tokenList.length];
}
static void appendChar(Token* token, char c) {
    token->content[token->length++] = c;
}
static void pushToken(e_TokenType type, char c) {
    Token* token = newEmptyToken(type);
    appendChar(token, c);
}
static void flushToken(char) {
    ++tokenList.length;
}
static void newNum(char c) {
    pushToken(t_num, c);
}
static void newNegNum(char c) {
    Token* token = newEmptyToken(t_num);
    appendChar(token, '-');
    appendChar(token, c);
}
static void addNum(char c) {
    Token* token = currentToken();
    appendChar(token, c);
}
static void convToDec(char c) {
    Token* token = currentToken();
    token->tokenType = t_decNum;
    appendChar(token, c);
}
static void newOperator(char c) {
    pushToken(t_operator, c);
}
static void newOpenBrace(char c) {
    pushToken(t_braceOpen, c);
}
static void newCloseBrace(char c) {
    pushToken(t_braceClose, c);
}
static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}
static bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
static bool isSub(char c) {
    return c == '-';
}
static bool isOpenBrace(char c) {
    return c == '[' || c == '{' || c == '(';
}
static bool isCloseBrace(char c) {
    return c == ']' || c == '}' || c == ')';
}
static bool isWhitespace(char c) {
    return c == ' ' || c == '\n' || c == '\r';
}
static bool isDecimalDot(char c) {
    return c == '.';
}
static bool wasOperator(TokenizerState state) {
    return state == s_operator;
}
static bool wasDigit(TokenizerState state) {
    return state == s_number;
}
static bool wasNeg(TokenizerState state) {
    return state == s_neg;
}
static bool wasPos(TokenizerState state) {
    return state == s_pos;
}
static bool wasBrace(TokenizerState state) {
    return state == s_braceOpen || state == s_braceClose;
}
static bool wasBraceOpen(TokenizerState state) {
    return state == s_braceOpen;
}
static bool wasBraceClose(TokenizerState state) {
    return state == s_braceClose;
}
static bool wasValidForOperator(TokenizerState state) {
    return wasDigit(state) || wasBraceClose(state);
}
static bool wasValidForCloseBrace(TokenizerState state) {
    return !wasOperator(state);
}
static bool wasValidForOpenBrace(TokenizerState) {
    return true;
}
static bool wasAny(TokenizerState) {
    return true;
}

static void procChar(char c) {
    static const Transition transitions[] = {
        {wasBrace, isDigit, s_number, newNum},
        {wasNeg, isDigit, s_number, newNegNum},
        {wasOperator, isDigit, s_number, newNum},
        {wasPos, isDigit, s_number, newNum},
        {wasDigit, isDigit, s_number, addNum},

        {wasValidForOperator, isOperator, s_operator, newOperator},
        {wasValidForOpenBrace, isOpenBrace, s_braceOpen, newOpenBrace},
        {wasValidForCloseBrace, isCloseBrace, s_braceClose, newCloseBrace},

        {wasDigit, isDecimalDot, s_number, convToDec},
        {wasBraceOpen, isSub, s_neg},
        {wasOperator, isSub, s_neg},
        {wasNeg, isSub, s_pos},
        {wasPos, isSub, s_neg},

        {wasAny, isWhitespace, s_done, flushToken},
    };

    for (int i = 0; i < sizeof(transitions) / sizeof(Transition); i++) {
        if (transitions[i].prevState && transitions[i].event && transitions[i].prevState(tokenizerState) && transitions[i].event(c)) {
            if (transitions[i].actionFun) transitions[i].actionFun(c);
            if (transitions[i].nextState != s_invalid) tokenizerState = transitions[i].nextState;
            break;
        }
    }
}

