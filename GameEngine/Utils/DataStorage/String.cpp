//
// Created by ggmfrankie on 11/17/26.
//

#include "String.hpp"

namespace ggm {
    namespace {
        void test() {
            std::string stst {"sdasd"};
            String s = "323";
            String s2 = stst;
            s.append("it is working");

            s2.contains("asda");

        }
    }

    String::String(const String &other) :
        mData(other.mData)
    {}

    String::String(String &&other) noexcept :
        mData(std::move(other.mData))
    {}

    size_t String::size() const {
        return mData.size();
    }

    size_t String::capacity() const {
        return mData.capacity();
    }

    const char* String::data() const {
        return mData.data();
    }

    const char* String::cStr() const {
        return mData.c_str();
    }

    String String::substr(size_t from) const {
        return {mData.substr(from)};
    }

    String String::substr(size_t from, size_t to) const {
        return {mData.substr(from, to)};
    }

    bool String::isEmpty() const {
        return mData.empty();
    }
} // ggm