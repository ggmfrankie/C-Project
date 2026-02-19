//
// Created by ertls on 19.11.2025.
//

#include "Personalverwaltung.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

Personal createPersonal() {
    Personal personal = {};

    puts("Geben Sie einen Namen ein:\n");
    scanf("%s", personal.name);

    puts("Geben Sie ein Alter ein:\n");
    scanf("%d", &personal.age);

    puts("Geben Sie einen Lohn ein:\n");
    scanf("%d", &personal.wage);

    return personal;
}

void addTestPersonal(List_Personal *list_personal) {
    Personal_ListAdd(list_personal,
        (Personal){.name = "Herbert", .age = 20, .wage = 2000}
    );
    Personal_ListAdd(list_personal,
        (Personal){.name = "Sepp", .age = 23, .wage = 2020}
    );
    Personal_ListAdd(list_personal,
        (Personal){.name = "Gilbert", .age = 40, .wage = 10000}
    );
}

void initPersonal(List_Personal *listPersonal) {
    int again;

    Again:
    again = 0;
    Personal pe = createPersonal();
    Personal_ListAdd(listPersonal, pe);
    puts("Again?\n");
    scanf("%i", &again);

    if (again) goto Again;
}

void printPerson(Personal* personal) {
    printf("Name: %s\n", personal->name);
    printf("Age: %d\n", personal->age);
    printf("Wage: %d\n\n", personal->wage);
}

float durchschnittsGehalt(const List_Personal* listPersonal) {
    int total = 0;
    return (float) total / (float) listPersonal->size;
}

void printPersonal(List_Personal* listPersonal) {
    Personal_iterator(listPersonal, printPerson);
}

void personalVerwaltung() {
    List_Personal personalList = Personal_newList(16);
    addTestPersonal(&personalList);
    initPersonal(&personalList);
    printPersonal(&personalList);

    const float durchschnitt = durchschnittsGehalt(&personalList);
    printf("Durchschnittsgehalt: %.2f\n", durchschnitt);
}
