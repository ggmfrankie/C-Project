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

inline struct {
    struct {
        void println(String s = "");
    } out;
} System;

Public interface IPrintable {

};

package spam {
    Public class Java {
        private int a = 0;
        private String name;

        public Java(String name) {
            this->name = name;
        }

        public String getName() {
            return this->name;
        }

        public void print() {
            System.out.println(name);
        }
    };
} // spam

#undef package
#undef public
#undef private
#undef Public
#undef var
#undef interface
#undef extends
#undef implements