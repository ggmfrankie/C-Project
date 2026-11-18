//
// Created by Stefan on 17.11.2026.
//

#pragma once
#include <cstring>
#include <string>

#include "ggmdef.hpp"
#include "String.hpp"
#include "Debug/Assert.hpp"

namespace ggm {
    class StringView {
        const char* mData;
        u64 mSize;

    public:
        StringView() = delete;
        StringView(const std::string& other);
        StringView(const String& other);
        StringView(const char* cStr);

        void removePrefix(u64 length);
        void removePrefix(const char* prefix);

        template<u64 N>
        void removePrefix(const char (&prefix)[N+1]) {
            const char* it = mData + N;

            Assert::run((it - begin()) >= N);

            if (memcmp(mData, prefix, N) > 0) return;
            mData += N;
        }

        void removePostfix(u64 length);
        void removePostfix(const char* postfix);

        template<u64 N>
        void removePostfix(const char (&prefix)[N+1]) {
            const char* it = mData + mSize-1 - N;

            Assert::run((end() - it) > N);

            if (memcmp(it, prefix, N) > 0) return;
            mSize -= N;
        }

        using Iterator = const char*;

        Iterator begin() const;
        Iterator end() const;
    };
} // ggm
