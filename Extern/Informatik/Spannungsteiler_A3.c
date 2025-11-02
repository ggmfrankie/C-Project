//
// Created by Stefan on 02.11.2025.
//

#include "Spannungsteiler_A3.h"

#include <bemapiset.h>
#include <stdio.h>
#define Diode 2.5
int Berechnung() {
    Spannung U = 5.0f;
    Widerstand R1 = 100.0f;
    Widerstand R2 = 150.0f;
    Widerstand RL = 50.0f;

    Spannung U2 = berechneSpannungsteiler(U, R1, berechneErsatzwiderstand(R2, RL));
    printf("U : %6.2f \n", U);
    printf("R1: %6.2f \n", R1);
    printf("R2: %6.2f \n", R2);
    printf("RL: %6.2f \n", RL);
    printf("U2: %6.2f \n", U2);
}

Spannung berechneSpannungsteiler(const Spannung U, const Widerstand R1, const Widerstand R2) {
    if (U < 0 || R1 < 0 || R2 < 0) return -1;
    const Widerstand ergebnis = (R2 / (R1+R2)) * U;

    return min(ergebnis, Diode);
}

inline Widerstand berechneErsatzwiderstand(const Widerstand R2, const Widerstand RL) {
    if (RL < 0 || R2 < 0) return -1;

    return (R2 * RL) / (R2 + RL);
}