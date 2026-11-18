//
// Created by ggmfrankie on 11/17/26.
//

#pragma once
#include <string>

namespace ggm {
    class String {
        std::string mData;

    public:
        String() = default;
        String(const String& other);
        String(String&& other) noexcept;

        template<typename S>
            requires requires (S& s) {std::string(s);}
        String(const S& other) :
            mData(other)
        {}

        template<typename S>
            requires requires (S&& s) {std::string(std::move(s));}
        String(S&& other) :
            mData(std::move(other))
        {}

        template<typename S>
            requires requires (S& s, std::string str) {str = s;}
        String& operator=(const S& other) {
            mData = other;
            return *this;
        }

        size_t size() const;
        size_t capacity() const;
        const char* data() const;
        const char* cStr() const;

        String substr(size_t from) const;
        String substr(size_t from, size_t to) const;

        bool isEmpty() const;

        template<typename S>
            requires requires (S& s, std::string str) {str.contains(s);}
        [[nodiscard]] bool contains(const S& s) {
            return mData.contains(s);
        }

        template<typename S>
            requires requires (S& s, std::string str) {str.append(s);}
        String& append(const S& s) {
            mData.append(s);
            return *this;
        }

        template<typename Self>
        auto& operator[](this Self&& self, size_t index) {
            return self.mData[index];
        }

        template<typename Self>
        auto begin(this Self&& self) {
            return self.mData.begin();
        }

        template<typename Self>
        auto end(this Self&& self) {
            return self.mData.end();
        }

        auto operator<=>(const String&) const = default;
    };

} // ggm
