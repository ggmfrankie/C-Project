//
// Created by Stefan on 11.10.2026.
//

#include "Test.hpp"

#include <iostream>
#include <print>

#include "DataStorage/HashMap.hpp"

namespace Testing {
    static void testHashMap() {
        ggm::HashMap<std::string, int> map {};

        map.insert("hassan", 54);

        int value = *map["hassan"];

        assert(value == 54);

        map.insert("kek", 545);

        for (const auto& object : map) {

        }
    }

    void runTests() {
        testHashMap();
    }
}


