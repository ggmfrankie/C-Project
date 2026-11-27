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
        constexpr StringView() = delete;
        constexpr StringView(const std::string& other);
        constexpr StringView(const String& other);
        constexpr StringView(const char* cStr);

        constexpr void removePrefix(u64 length);
        constexpr void removePrefix(const char* prefix);

        template<u64 N>
        constexpr void removePrefix(const char (&prefix)[N+1]) {
            const char* it = mData + N;

            Assert::check((it - begin()) >= N);

            if (memcmp(mData, prefix, N) > 0) return;
            mData += N;
        }

        constexpr void removePostfix(u64 length);
        constexpr void removePostfix(const char* postfix);

        template<u64 N>
        constexpr void removePostfix(const char (&prefix)[N+1]) {
            const char* it = mData + mSize-1 - N;

            Assert::check((end() - it) > N);

            if (memcmp(it, prefix, N) > 0) return;
            mSize -= N;
        }

        using Iterator = const char*;

        constexpr Iterator begin() const;
        constexpr Iterator end() const;
    };
} // ggm

constexpr ggm::StringView operator""_SV(const char* literal);
