//
// Created by Stefan on 17.11.2026.
//

#include "StringView.hpp"

#include <cstring>
#include <string_view>

namespace ggm {
    namespace {
        void test() {
            std::string_view s = "dfaf";
            auto c = s.data();

            StringView a = "adfa";
            a = "sada";
        }
    }

    constexpr StringView::StringView(const std::string& other) :
        mData(other.data()),
        mSize(other.size())
    {}

    constexpr StringView::StringView(const String& other) :
        mData(other.data()),
        mSize(other.size())
    {}

    constexpr StringView::StringView(const char* cStr) :
        mData(cStr),
        mSize(strlen(cStr))
    {}

    constexpr void StringView::removePrefix(u64 length) {
        Assert::check(mSize >= length);
        mData += length;
    }

    constexpr void StringView::removePrefix(const char* prefix) {
        const u64 len = strlen(prefix);
        const char* it = mData + len;

        Assert::check((it - begin()) >= len);

        if (memcmp(mData, prefix, len) > 0) return;
        mData += len;
    }

    constexpr void StringView::removePostfix(u64 length) {
        Assert::check(mSize >= length);
        mSize -= length;
    }

    constexpr void StringView::removePostfix(const char* postfix) {
        const u64 len = strlen(postfix);
        const char* it = mData + mSize-1 - len;

        Assert::check((end() - it) > len);

        if (memcmp(it, postfix, len) > 0) return;
        mSize -= len;
    }

    constexpr StringView::Iterator StringView::begin() const {return mData;}
    constexpr StringView::Iterator StringView::end() const {return mData + mSize;}
} // ggm

constexpr ggm::StringView operator""_SV(const char* literal) {
    return {literal};
}
