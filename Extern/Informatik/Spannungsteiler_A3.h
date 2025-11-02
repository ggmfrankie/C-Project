//
// Created by Stefan on 02.11.2025.
//

#ifndef C_SPANNUNGSTEILER_A3_H
#define C_SPANNUNGSTEILER_A3_H
typedef double Spannung;
typedef double Widerstand;

Spannung berechneSpannungsteiler(Spannung U, Widerstand R1, Widerstand R2);
Widerstand berechneErsatzwiderstand(Widerstand R2, Widerstand RL);
int Berechnung();

#endif //C_SPANNUNGSTEILER_A3_H