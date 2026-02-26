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
} e_TokenType;

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

typedef struct s_Operation{
    bool hasArgs;
    double arg1;
    double arg2;

    OperatorType type;
    int weight;

    struct s_Operation* parentLeft;
    struct s_Operation* parentRight;
} Operation;

static void processChar(char c);
static void procChar(char c);
static OperatorType getOperatorType(char c);
static double parseTokens();

static Token token[512];
static TokenizerState tokenizerState = braceOpen;
static int length = -1;

static void printTokens();

void runParser() {

    do {
        tokenizerState = braceOpen;
        char input[512] = {};
        length = -1;
        memset(token, 0, sizeof(token));
        printf("Enter an expression:");
        scanf("%512s", input);

        for (int i = 0; i <= strlen(input); i++ ) {
            procChar(input[i]);
        }
        length++;
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

double evaluateOperations(Operation* op) {
    double result = evaluate(op->type, op->arg1, op->arg2);
    if (op->parentLeft && !op->parentRight) {
        op->parentLeft->arg2 = result;
    } else if (op->parentRight && !op->parentLeft) {
        op->parentRight->arg1 = result;
    } else if (op->parentLeft) {
        Operation *op1 = op->parentLeft;
        Operation *op2 = op->parentRight;
        if (op1->weight >= op2->weight) {
            op1->parentRight = op2;
        } else {

        }
    }
}

/*

static double parseTokens() {
    static int weights[] = {5, 10, 15, 20};
    Operation operations[512] = {};

    double numbers[512] = {};
    int numberIndex = 0;

    int opIndex = 0;
    int depth = 0;

    for (int i = 0; i < length; i++) {

        Token t = token[i];
        if (t.tokenType == t_operator) {

            OperatorType type = getOperatorType(t.content[0]);
            operations[opIndex].type = type;
            operations[opIndex++].weight = weights[type] + depth;

        } else if (t.tokenType == t_braceOpen) {
            depth += 100;
        } else if (t.tokenType == t_braceClose) {
            depth -= 100;
        }
    }

    for (int i = 0; i < length; i++) {
        Token t = token[i];
        if (!(t.tokenType == t_num || t.tokenType == t_decNum)) continue;
        if (t.tokenType == t_num) {
            numbers[numberIndex] = (double)strtol(t.content, NULL, 0);
        } else {
            numbers[numberIndex] = strtod(t.content, NULL);
        }

        if (numberIndex == 0) {
            operations[numberIndex].arg1 = &numbers[numberIndex];
            operations[numberIndex].t_arg1 = num;
        } else if (numberIndex == opIndex) {
            operations[numberIndex-1].arg2 = &numbers[numberIndex];
            operations[numberIndex-1].t_arg2 = num;
        }
        else {
            Operation* op2 = &operations[numberIndex];
            Operation* op1 = &operations[numberIndex - 1];
            if (op1->weight >= op2->weight) {
                op1->arg2 = &numbers[numberIndex];
                op1->t_arg2 = num;
                op2->arg1 = op1;
                op1->parentLeft = op2;
                op2->t_arg1 = expr;
            } else {
                op2->arg1 = &numbers[numberIndex];
                op2->t_arg1 = num;
                op1->arg2 = op2;
                op2->parentRight = op1;
                op1->t_arg2 = expr;
            }
        }
        numberIndex++;

    }

    int lowest = operations[0].weight;
    int lowestIndex = 0;
    for (int i = 1; i < opIndex; i++) {
        if (operations[i].weight <= lowest) {
            lowestIndex = i;
            lowest = operations[i].weight;
        }
    }
    return evaluateOperations(&operations[lowestIndex]);
}
*/

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

