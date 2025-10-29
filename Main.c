
#include <stdio.h>

#include "Render/Engine.h"
#include "Utils/Map.h"
#include "Utils/String.h"


void linkedListStuff();
MAP(Test, String, int);

int main(){
    printf("Starting Engine!\n");
    Map_Test map = newMap_Test(16, str_equals);
    map.put(&map, newString("Hellow"), 20);
    printf("%i\n", map.get(&map, newString("Hellow")));
    startEngine();
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
    String string = newString("Hello my Firend");
    str_println(&string);
    List_String words = string.split(&string, " ");
    for (int i = 0; i < words.size; i++) {
        str_println(words.get(&words, i));
    }
}
*/
