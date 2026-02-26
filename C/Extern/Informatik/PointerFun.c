//
// Created by ertls on 19.11.2025.
//

#include "PointerFun.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <_bsd_types.h>
#include "../../Utils/FileIO.h"

void testEndianness();
void set4711(int *pI);
void set4712(int i);
void decrypt(String* text, const String* key);

void pointerFun() {
    testEndianness();

    int i = 0x04030201;
    int *pi = NULL;
    short *ps = NULL;
    char *pc = NULL;
    int vi[] = {0, 1, 2, 3};
    char vc[] = "abcdefghijklmno";

    printf("\nENDE\n");

    pi = &i;
    puts("Assign pi to &i");
    printf("%p -> %08x\n", pi, *pi);

    puts("Doing some bad stuff");
    //ps = &i; //expects 2 bytes
    //pc = &i; //expects 1 byte

    puts("casting");
    ps = (short*)&i; //expects 2 bytes
    pc = (char*)&i; //expects 1 byte

    puts("Accessing i with wrong pointers");
    printf("ps -> %08x\npc -> %08x\n", *ps, *pc);

    puts("Setting *ps to a new value");
    *ps = 0x4433;
    printf("ps -> %08x\ni -> %08x\n", *ps, i);

    puts("Setting *pc to a new value");
    *pc = 'X';
    printf("pc -> %08x\ni -> %08x\n", *pc, i);

    puts("\nArrays:");
    printf("&vi -> %p\n&vc -> %p\n", vi, vc);

    puts("*pi = vi[0] and *pc = vc[0]");
    pi = vi;
    pc = vc;
    printf("pi -> %p\npc -> %p\n", pi, pc);

    printf("%i", vc == &vc[0]);
    puts("Incrementing the pointer pi and pc");
    pi++;
    pc++;
    printf("pi -> %p\npc -> %p\n", pi, pc);

    puts("Rätsel");

    puts("char* pc -> int* pc -> int* pc+1 -> char* pc+4 -> getValue");
    printf("vc[5] = %c\n*((char*)(((int*)pc)+1)) = %c\n", vc[5], *((char*)(((int*)pc)+1)));

    puts("Pointer in functions");
    set4711(&i);
    printf("i -> %i\n", i);
    set4712(i);
    printf("i -> %i\n", i);

    puts("String shenanigans");
    char charArray[512];

    puts("Geben sie ein Wort ein:");
    scanf("%511s", charArray);
    const int length = (int)strlen(charArray);
    char reversedCharArray[length + 1] = {};

    for (int k = 0; k < length; k++) {
        reversedCharArray[k] = charArray[length-(k+1)];
    }
    reversedCharArray[length] = '\0';
    printf("Normal: %s\nReversed: %s\n", charArray, reversedCharArray);

    const String filepath = stringOf("../Extern/Informatik/Data/03_geheim.txt");
    String file = readFile(&filepath);
    const String key = stringOf("Erdbeermilch");
    const String destinationFile = stringOf("../Extern/Informatik/Data/entschluesselt.txt");
    Strings.println(&file);

    decrypt(&file, &key);

    printf("%s\n", file.content);
    writeFile(&destinationFile, &file);
    puts("finito");
}

void testEndianness() {
    // erstes byte wird mit ff gefüllt, alle anderen bytes 0
    u_int endianTest = 255;
    const u_char *intBytes = (unsigned char*)&endianTest;
    printf("%s\n", intBytes[0] ? "littleEndian" : "bigEndian");
    puts("\n255:");
    printf("First Byte: %x\n", intBytes[0]);
    printf("Second Byte: %x\n", intBytes[1]);

    puts("\n256:");
    // erstes byte overflowed und wird zu 00, zweites byte wird 01
    endianTest++;
    printf("First Byte: %x\n", intBytes[0]);
    printf("Second Byte: %x\n", intBytes[1]);

    int testInt = 0x04030201;

    puts("Testing with new int");
    const u_char *integer = (u_char*)&testInt;
    for (int j = 0; j < sizeof(int); ++j) {
        printf("%x\n", integer[j]);
    }

    puts("-------------------------------");
}

void decrypt(String* text, const String* key) {
    const size_t length = text->length;
    int keyIndex = 0;
    for (int i = 0; i < length; i++) {
        char c = text->content[i];

        if (c < 32) {
            if (c == '\n') keyIndex = 0;
            continue;
        }

        u_char kLow = (u_char) (key->content[keyIndex % key->length] & 0x0F);
        keyIndex++;
        u_char cLow = (u_char) (c & 0x0F);

        u_char newC = (u_char)((kLow ^ cLow) & 0x0F);

        text->content[i] = (char)((c & 0xF0) | newC);
    }
}

void set4711(int *pI)
{
    printf("\nIn 4711 vor  Zuweisung:\n Add: %p Inh: %i",pI,*pI);
    *pI = 4711;
    printf("\nIn 4711 nach Zuweisung:\n Add: %p Inh: %i",pI,*pI);
    printf("\n\n");
}

void set4712(int i)
{
    printf("\nIn 4712 vor  Zuweisung:\n Add: %p Inh: %i",&i,i);
    i = 4712;
    printf("\nIn 4712 nach  Zuweisung:\n Add: %p Inh: %i",&i,i);
    printf("\n\n");
}
