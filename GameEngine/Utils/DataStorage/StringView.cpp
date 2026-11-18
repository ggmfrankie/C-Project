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

    StringView::StringView(const std::string& other) :
        mData(other.data()),
        mSize(other.size())
    {}

    StringView::StringView(const String& other) :
        mData(other.data()),
        mSize(other.size())
    {}

    StringView::StringView(const char* cStr) :
        mData(cStr),
        mSize(strlen(cStr))
    {}

    void StringView::removePrefix(u64 length) {
        Assert::run(mSize >= length);
        mData += length;
    }

    void StringView::removePrefix(const char* prefix) {
        const u64 len = strlen(prefix);
        const char* it = mData + len;

        Assert::run((it - begin()) >= len);

        if (memcmp(mData, prefix, len) > 0) return;
        mData += len;
    }

    void StringView::removePostfix(u64 length) {
        Assert::run(mSize >= length);
        mSize -= length;
    }

    void StringView::removePostfix(const char* postfix) {
        const u64 len = strlen(postfix);
        const char* it = mData + mSize-1 - len;

        Assert::run((end() - it) > len);

        if (memcmp(it, postfix, len) > 0) return;
        mSize -= len;
    }

    StringView::Iterator StringView::begin() const {return mData;}
    StringView::Iterator StringView::end() const {return mData + mSize;}
} // ggm