//
// Created by ertls on 13.03.2026.
//

#pragma once
#include <string>


namespace Game {
    class GameLayer {
    private:
        int age;
        int ahhsa;
    public:
        GameLayer(int age) {
            this->age = age;
            ahhsa = 6;
        }
    };

    struct Human {
    public:
        std::string name;
        int age;

    public:
        Human(std::string n) {
            name = n;
            age = 0;
        }

        void setAge(int a) {
            age = a;
        }
    };



    void test() {
        GameLayer gm(7);

        Human h{"Sepp"};

        h.setAge(33);

        Human* hp = new Human("haa");


        delete hp;
    }
} // Game