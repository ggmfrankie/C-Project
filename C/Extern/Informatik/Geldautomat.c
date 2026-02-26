//
// Created by ertls on 12.11.2025.
//

#include "Geldautomat.h"
#include "../../Utils/TimeMeasurenments.h"

#include <stdio.h>
typedef int Geld;
typedef struct ReturnValues {
    int count;
    int remainder;
} ReturnValues;

void berechneTeilsumme(int* ausgabe, int input);
void berechneTeilsummeMitStruct(int* ausgabe, int input);
ReturnValues gibAnzahlGeld(int restSumme, int scheinWert);
void printScheinWerte(const int* ausgabe);

typedef enum {
    e500 = 0,
    e200 = 1,
    e100 = 2,
    e50 = 3,
    e20 = 4,
    e10 = 5,
    e5 = 6,
    e2 = 7,
    e1 = 8,
    c50 = 9,
    c20 = 10,
    c10 = 11,
    c5 = 12,
    c2 = 13,
    c1 = 14
}Scheine;

void geldautomatTest() {
    int ergebnisse[GELD_ANZAHL] = {};
    int ergebnisse2[GELD_ANZAHL] = {};
    const Geld geld = 1434123939;

    berechneTeilsummeMitStruct(ergebnisse2, geld);
    printScheinWerte(ergebnisse2);

    berechneTeilsumme(ergebnisse, geld);
    printScheinWerte(ergebnisse);
}

void berechneTeilsumme(int* ausgabe, int input) {
    printf("\nGeld: %d,%d€ \n", input/100, input%100);
    const TimeNs startTime = now_ns();
    static const int scheinWerte[GELD_ANZAHL] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10 ,5 , 2, 1};

    Scheine schein = e500;

    while (input != 0) {
        const int scheinWert = scheinWerte[schein];
        while (input >= scheinWert) {
            input -= scheinWert;
            ausgabe[schein]++;
            if (input == 0) {
                printf("Time to Execute: %lli\n", now_ns() - startTime);
                return;
            }
        }
        schein++;
    }
}

void berechneTeilsummeMitStruct(int* ausgabe, const int input) {
    printf("\nGeld: %d,%d€ \n", input/100, input%100);
    const TimeNs startTime = now_ns();
    static const int scheinWerte[GELD_ANZAHL] = {50000, 20000, 10000, 5000, 2000, 1000, 500, 200, 100, 50, 20, 10 ,5 ,2, 1};

    ReturnValues rv = {
        .count = 0,
        .remainder = input
    };

    for (Scheine i = 0; i < GELD_ANZAHL; i++) {
        rv = gibAnzahlGeld(rv.remainder, scheinWerte[i]);
        ausgabe[i] = rv.count;
        if (rv.remainder == 0) break;
    }
    printf("Time to Execute with struct: %lli\n", now_ns() - startTime);
}

inline ReturnValues gibAnzahlGeld(const int restSumme, const int scheinWert) {
    return (ReturnValues){
        .count = restSumme / scheinWert,
        .remainder = restSumme % scheinWert
    };
}

void printScheinWerte(const int* ausgabe) {
    static const char* labels[GELD_ANZAHL] = {
        "500€", "200€", "100€", "50€", "20€",
        "10€", "5€", "2€", "1€",
        "50ct", "20ct", "10ct", "5ct", "2ct", "1ct"
    };

    for (int i = 0; i < GELD_ANZAHL; i++) {
        if (ausgabe[i] == 0) continue;

        printf("%s: %i\n", labels[i], ausgabe[i]);
    }
}