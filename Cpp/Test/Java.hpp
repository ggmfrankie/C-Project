//
// Created by ertls on 20.04.2026.
//

#pragma once
#include <string>
#define package namespace
#define public public:
#define private private:
#define Public
#define var auto;
#define interface class
#define extends :
#define implements ,

using String = std::string;

Public interface IPrintable {

};


package spam {
    Public class Java {
        public int a;
        public String name;

        public Java(String name) {
            this->name = name;
        }

        public String getName() {
            return this->name;
        }
    };
} // spam