//
// Created by Stefan on 17.11.2026.
//

#pragma once
#include <string>
#include <format>

#include "ggmdef.hpp"
#include "String.hpp"
#include "Debug/Assert.hpp"
#include <cassert>

namespace ggm {
    class String;

    class StringView {
        using Iterator = const char*;

        const char* mData = nullptr;
        u64 mSize = 0;

    public:
        constexpr StringView() = delete;

        constexpr StringView(const std::string& other) :
            mData(other.data()),
            mSize(other.size())
        {}

        constexpr StringView(const String& other) :
            mData(other.data()),
            mSize(other.size())
        {}

        constexpr StringView(const char* cStr) :
            mData(cStr),
            mSize(std::char_traits<char>::length(cStr))
        {}

        constexpr StringView(Iterator start, u64 len) :
            mData(start),
            mSize(len)
        {}

        constexpr StringView(Iterator start, Iterator end) :
            mData(start),
            mSize([start, end]{assert(end > start); return end - start;}())
        {}

        constexpr StringView(const std::string_view sv) :
            mData(sv.data()),
            mSize(sv.size())
        {}

        constexpr const char* data() const {
            return mData;
        }

        constexpr u64 size() const {
            return mSize;
        }

        constexpr void removePrefix(u64 length) {
            Assert::check(mSize >= length);
            mData += length;
        }

        constexpr void removePrefix(const char* prefix) {
            const u64 len = strlen(prefix);
            const char* it = mData + len;

            Assert::check((it - begin()) >= len);

            if (memcmp(mData, prefix, len) > 0) return;
            mData += len;
        }

        template<u64 N>
        constexpr void removePrefix(const char (&prefix)[N+1]) {
            const char* it = mData + N;

            Assert::check((it - begin()) >= N);

            if (memcmp(mData, prefix, N) > 0) return;
            mData += N;
        }

        constexpr void removePostfix(u64 length) {
            Assert::check(mSize >= length);
            mSize -= length;
        }

        constexpr void removePostfix(const char* postfix) {
            const u64 len = strlen(postfix);
            const char* it = mData + mSize-1 - len;

            Assert::check((end() - it) > len);

            if (memcmp(it, postfix, len) > 0) return;
            mSize -= len;
        }

        template<u64 N>
        constexpr void removePostfix(const char (&prefix)[N+1]) {
            const char* it = mData + mSize-1 - N;

            Assert::check((end() - it) > N);

            if (memcmp(it, prefix, N) > 0) return;
            mSize -= N;
        }

        constexpr friend std::ostream& operator<<(std::ostream& os, const StringView& sv) {
            return os.write(sv.mData, static_cast<std::streamsize>(sv.mSize));
        }

        constexpr Iterator begin() const {return mData;}
        constexpr Iterator end() const {return mData + mSize;}
    };
} // ggm

template<>
    struct std::formatter<ggm::StringView> : std::formatter<std::string_view> {
    auto format(const ggm::StringView& sv, std::format_context& ctx) const {
        return std::formatter<std::string_view>::format(
            std::string_view(sv.data(), sv.size()), ctx
        );
    }
};
