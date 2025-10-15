
#include <stdio.h>

#include "Render/Engine.h"
#include "Utils/String.h"


void linkedListStuff();
void readInputs();

int main(){
    printf("Starting Engine!\n");
    readInputs();
    //startEngine();
}

void readInputs() {
    int a;
    int b;

    printf("Enter your choice : ");
    scanf("%i",&a);
    printf("Enter another choice : ");
    scanf("%i",&b);
    printf("Result: %i + %i = %i\n", a, b, a+b );

    char input[1024];
    printf("Enter your Name: ");
    scanf("%s",input);
    printf("Hello %s!\n",input);

}









/*
void linkedListStuff(){
    
    LinkedList_int intList = newList_int();
    LinkedList_String stringList = newList_String();

    String name = newString("Hubert");
    String name2 = newString("herbert");

    addData_int(&intList, 2);
    addData_int(&intList, 34434);

    //addData_String(&stringList, name);
    //addData_String(&stringList, name2);
    
    printf("Hello is this working?\n");
    //String *substr = substring(getDataAt_String(&stringList, 0), 1,3);
    //println(substr);
}
*/
