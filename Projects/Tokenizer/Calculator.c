//
// Created by ertls on 12.02.2026.
//

#include "Calculator.h"

#include <stdio.h>
#include <string.h>

static void processChar(char c);

typedef enum {
    singleMode,
    accumulateNumber,
    accumulateBraces
} TokenizerState;

typedef enum {
    operatorToken,
    numberToken,
    bracesToken,
} TokenType;

typedef struct {
    char content[512];
    int length;
    TokenType tokenType;
} Token;

static Token token[512];
static int length = 0;

static void printTokens();

void runParser() {
    char* string = "(1+3*400)-402 +71";
    for (int i = 0; i < strlen(string)+1; i++ ) {
        processChar(string[i]);
    }
    printTokens();
}

static void printTokens() {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < token[i].length; j++) {
            printf("%c", token[i].content[j]);
        }
        puts("");
    }
}


static void processChar(char c) {
    static TokenizerState state = singleMode;
    switch (state) {
        case singleMode:
            if (c >= '0' && c <= '9') {
                state = accumulateNumber;
                token[length].tokenType = numberToken;
                goto NUM;
            } else if (c == '+'|| c == '-' || c == '*' || c == '/') {
                token[length].tokenType = operatorToken;
                token[length].content[token[length].length++] = c;
                length++;
            } else if (c == '(') {
                state = accumulateBraces;
                token[length].tokenType = bracesToken;
                goto BRAC;
            }
            break;
        case accumulateNumber:
            if (c >= '0' && c <= '9') {
                NUM:
                token[length].content[token[length].length++] = c;
            } else {
                length++;
                state = singleMode;
            }
            break;
        case accumulateBraces:
            if (c == ')') {
                token[length].content[token[length].length++] = c;
                length++;
                state = singleMode;
            } else {
                BRAC:
                token[length].content[token[length].length++] = c;
            }
        break;
    }
}