//
// Created by Stefan on 28.09.2026.
//

export module String;
#include <concepts>
#include <cstring>

export class String {
    char* mData {};

    String(const char* data) {
        const auto len = std::strlen(data);

        mData = new char[len+1];
        std::strcpy(mData, data);
        mData[len] = '\0';
    }
};

export auto add(std::integral auto a, std::integral auto b) {
    return a + b;
}