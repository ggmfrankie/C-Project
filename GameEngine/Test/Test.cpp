//
// Created by Stefan on 29.03.2026.
//

#include "Test.hpp"

namespace {
    struct Int {
        int i;
        Int(int i): i(i){}

        void* operator&() const {
            return nullptr;
        }

        void* operator->*(auto k) {
            return nullptr;
        }

        void operator->() {

        }
    };

    float operator,(Int a, int b) {
        float f = b;
        while (f > 1.0) {
            f/=10;
        }

        return static_cast<float>(a.i) + f;
    }

    void* operator&(int i, Int) {
            return nullptr;
    }

    void test() {
        float i = (Int{1}, 4);

        struct Proxy {
            int value = 42;
        };

        struct Proxy2 {
            Proxy* operator->() {
                static Proxy p;
                return &p;
            }
        };

        struct Evil {
            const char* operator->*(int i) const && {
                return "hello world";
            }

            Evil operator->*(const std::string& anweisung) const {
                if (anweisung == "copy") return *this;
                return {};
            }

            const Evil& operator->*(const std::string& anweisung) && {
                if (anweisung == "Copy") return *this;
                return {};
            }
        };



        Evil e;
        const char* c = std::move(e)->*2;

        auto dsfaf = e->*"copy";

        auto* asdf = &(pthread_create_wrapper(reinterpret_cast<void*>(&e)), Int{2});

        auto dsasdf = std::move(e)->*"Copy";
    }


}

