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

        int v = *map["hassan"];

        assert(v == 54);

        map.insert("kek", 545);

        for (const auto& [key, value] : map) {
            (void)value;
        }

        for (auto it = map.begin(); it != map.end(); ++it) {
        }

        map.remove("kek");
    }

    void runTests() {
        testHashMap();
    }
}


