
#include <stdio.h>

#include "Render/Engine.h"
#include "Utils/String.h"


void linkedListStuff();

int main(){
    printf("Starting Engine!\n");
    startEngine();
    String string = newString("Hello my Firend");
    str_println(&string);
    List_String words = string.split(&string, " ");
    for (int i = 0; i < words.size; i++) {
        str_println(words.get(&words, i));
    }
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
