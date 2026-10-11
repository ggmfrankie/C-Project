//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include "ArrayList.hpp"

namespace ggm {
    template<typename Key, typename Value, typename Hash>
    class HashMap {
        struct Object {
            Key key;
            Value value;
        };

        ArrayList<u32> mHashTable;
        Hash mHash;
    };
} // ggm
