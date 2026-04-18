//
// Created by ertls on 12.02.2026.
//

#include "Calculator.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Extern/Libraries/WinAdapt.h"

typedef enum {
    invalid,
    neg,
    pos,
    operator,
    number,
    braceOpen,
    braceClose,
    error
} TokenizerState;


typedef enum {
    t_operator,
    t_num,
    t_decNum,
    t_braceOpen,
    t_braceClose,
} e_TokenType;j

typedef struct {
    char content[512];
    int length;
    e_TokenType tokenType;
} Token;

typedef _Bool(*isState)(TokenizerState state);
typedef _Bool(*isChar)(char c);
typedef void(*ActionFun)(char c);

typedef struct {
    isState prevState;
    isChar event;
    TokenizerState nextState;
    ActionFun actionFun;
} Transition;

typedef struct {
    Token val[512];
    int length;
} TokenList;

typedef enum {
    t_add,
    t_sub,
    t_mul,
    t_div
} OperatorType;

typedef enum {
    num,
    expr
} ArgumentType;

static void processChar(char c);
static void procChar(char c);
static OperatorType getOperatorType(char c);
static double parseTokens();

static TokenList tokenList = {};

static TokenizerState tokenizerState = braceOpen;

static void printTokens();

void runParser() {

    do {
        tokenizerState = braceOpen;
        char input[512] = {};
        tokenList.length = -1;
        memset(tokenList.val, 0, sizeof(tokenList.val));
        printf("Enter an expression:");
        scanf("%511s", input);

        for (int i = 0; i <= strlen(input); i++ ) {
            procChar(input[i]);
        }

        //printTokens();
        //printf("= %lf\n", parseTokens());
    } while (1);
}

double evaluate(OperatorType c, double a, double b) {
    switch (c) {
        case t_add: return (a + b);
        case t_sub: return (a - b);
        case t_mul: return (a * b);
        case t_div: return (a / b);
        default: return 0;
    }
}

static double parseTokens() {
    static int weights[] = {5, 10, 15, 20};

    int numberIndex = 0;

    int opIndex = 0;
    int depth = 0;

    for (int i = 0; i < )
}


static void printTokens() {
    static char* mapping[] = {"operatorToken", "numberToken", "decNumberToken", "braceOpenToken", "braceCloseToken"};
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < token[i].length; j++) {
            printf("%c", token[i].content[j]);
        }
        printf("\t: %s\n", mapping[token[i].tokenType]);
    }
}

static OperatorType getOperatorType(char c) {
    switch (c) {
        case '+': return t_add;
        case '-': return t_sub;
        case '*': return t_mul;
        case '/': return t_div;
        default: puts("invalid operator"); return t_add;
    }
}

static void pushToken(e_TokenType type, char c) {
    length++;
    token[length].tokenType = type;
    token[length].content[token[length].length++] = c;
}

static void newNum(char c) {
    pushToken(t_num, c);
}

static void newNegNum(char c) {
    length++;
    token[length].tokenType = t_num;
    token[length].content[token[length].length++] = '-';
    token[length].content[token[length].length++] = c;
}

static void addNum(char c) {
    token[length].content[token[length].length++] = c;
}

static void convToDec(char c) {
    token[length].tokenType = t_decNum;
    token[length].content[token[length].length++] = c;
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

static void printError(char c) {
    printf("Error! Encountered invalid char: %c\n", c);
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
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

static bool isAny(char c) {
    return true;
}

static bool wasOperator(TokenizerState state) {
    return state == operator;
}

static bool wasDigit(TokenizerState state) {
    return state == number;
}

static bool wasNeg(TokenizerState state) {
    return state == neg;
}

static bool wasPos(TokenizerState state) {
    return state == pos;
}

static bool wasBrace(TokenizerState state) {
    return state == braceOpen || state == braceClose;
}

static bool wasBraceOpen(TokenizerState state) {
    return state == braceOpen;
}

static bool wasBraceClose(TokenizerState state) {
    return state == braceClose;
}

static bool wasValidForOperator(TokenizerState state) {
    return wasDigit(state) || wasBraceClose(state);
}

static bool wasValidForCloseBrace(TokenizerState state) {
    return !wasOperator(state);
}

static bool wasValidForOpenBrace(TokenizerState state) {
    return true;
}

static bool wasAny(TokenizerState state) {
    return true;
}




static void procChar(char c) {

    static Transition transitions[] = {
        {wasBrace, isDigit, number, newNum},
        {wasNeg, isDigit, number, newNegNum},
        {wasOperator, isDigit, number, newNum},
        {wasPos, isDigit, number, newNum},
        {wasDigit, isDigit, number, addNum},


        {wasValidForOperator, isOperator, operator, newOperator},
        {wasValidForOpenBrace, isOpenBrace, braceOpen, newOpenBrace},
        {wasValidForCloseBrace, isCloseBrace, braceClose, newCloseBrace},

        {wasDigit, isDecimalDot, number, convToDec},
        {wasBraceOpen, isSub, neg},
        {wasOperator, isSub, neg},
        {wasNeg, isSub, pos},
        {wasPos, isSub, neg},

        {wasAny, isWhitespace},

        //{wasAny, isAny, error, printError}
    };

    for (int i = 0; i < sizeof(transitions) / sizeof(Transition); i++) {
        if (transitions[i].prevState && transitions[i].event && transitions[i].prevState(tokenizerState) && transitions[i].event(c)) {
            if (transitions[i].actionFun) transitions[i].actionFun(c);
            if (transitions[i].nextState != invalid) tokenizerState = transitions[i].nextState;
            break;
        }
    }
}

