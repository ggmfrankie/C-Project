//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include "ArrayList.hpp"
#include <concepts>
#include <functional>
#include <optional>
#include <algorithm>

#include "Debug/Exception.hpp"

namespace ggm {
    template<typename Key, typename Hash>
    concept HashType = requires(Hash hash, const Key& key) {
        { hash(key) } -> std::same_as<std::size_t>;
    };

    template<typename Key, typename Value, typename  Hash = std::hash<Key>>
    requires
        HashType<Key, Hash> &&
        std::equality_comparable<Key>
    class HashMap {
        struct Object {
            Key key;
            Value value;
        };

        ArrayList<i64> mHashTable;
        ArrayList<Object> mData;

        [[no_unique_address]] Hash mHash {};

        static constexpr i64 EMPTY_SLOT = -1;
        static constexpr int HASH_TABLE_FACTOR = 4;

        template<typename Self>
        constexpr auto getHashTableSlot(this Self&& self, const Key& key) {
            const size_t index = self.mHash(key) % self.mHashTable.capacity();

            const auto begin = self.mHashTable.begin();
            auto end = begin + self.mHashTable.capacity();

            auto it = begin + index;

            while (*it != EMPTY_SLOT) {
                if (self.mData[*it].key == key) return it;
                ++it;
                if (it == end) it = begin;
            }
            return end;
        }

        constexpr auto getEmptyHashTableSlot(const Key& key) {
            const size_t index = mHash(key) % mHashTable.capacity();

            const auto begin = mHashTable.begin();
            const auto end = begin + mHashTable.capacity();

            auto it = begin + index;
            while (*it != EMPTY_SLOT) {
                ++it;
                if (it == end) it = begin;
            }
            return it;
        }

        constexpr void rehash() {
            std::ranges::fill(mHashTable, EMPTY_SLOT);
            for (u64 i = 0; i < mData.size(); ++i) {
                auto&[key, value] = mData[i];

                if (const auto hashIndex = getEmptyHashTableSlot(key);
                    hashIndex != mHashTable.end()) {
                    *hashIndex = i;
                } else {
                    throw Exception({"Rehashing failed"});
                }
            }
        }

        constexpr void resizeHashTable(const u64 newCapacity) {
            mHashTable.growTo(newCapacity, -1);
            rehash();
        }

    public:
        constexpr explicit HashMap(const u64 capacity = 16) noexcept :
            mHashTable(capacity * HASH_TABLE_FACTOR, EMPTY_SLOT),
            mData(capacity)
        {}

        constexpr void insert(const Key& key, const Value& value) {
            // Check if hashtable needs resize
            if (static_cast<float>(mData.size()) / mHashTable.capacity() > 0.75) resizeHashTable(mHashTable.capacity() * 2);

            auto slot = getEmptyHashTableSlot(key);

            u64 index = mData.size();
            mData.push(key, value);

            *slot = index;
        }

        constexpr bool contains(const Key& key) const {
            return getHashTableSlot(key) != mHashTable.end();
        }

        constexpr Value* get(const Key& key) {
            const auto hashTableSlot = getHashTableSlot(key);
            return (hashTableSlot != mHashTable.end())
                ? &mData[*hashTableSlot].value
                : nullptr;
        }

        constexpr Value* operator[](const Key& key) {
            return get(key);
        }

        constexpr auto find(const Value& value) {
            return std::ranges::find(mData, value, &Object::value);
        }

        constexpr void remove(const Key& key) {
            auto slot = getHashTableSlot(key);

            const auto start = mHashTable.begin();
            const auto end = mHashTable.end();

            if (slot == end) return;

            const ssize_t index = *slot;
            *slot = EMPTY_SLOT;

            assert(end == mHashTable.begin() + mHashTable.capacity());
            assert(end > slot);

            auto probeStart = slot + 1;
            if (probeStart == end) probeStart = start;

            auto probe = probeStart;

            size_t blockSize = 0;

            while (*probe != EMPTY_SLOT) {
                ++blockSize;
                ++probe;
                if (probe == end) probe = start;
            }

            const auto probeEnd = probe;
            probe = probeStart;

            size_t rehashStack[blockSize];
            size_t rehashStackSize = 0;

            while (probe != probeEnd) {
                rehashStack[rehashStackSize++] = *probe;
                *probe = EMPTY_SLOT;

                ++probe;
                if (probe == end) probe = start;
            }

            for (size_t i = 0; i < rehashStackSize; ++i) {
                const size_t dataIndex = rehashStack[i];
                auto foundSlot = getEmptyHashTableSlot(mData[dataIndex].key);

                *foundSlot = dataIndex;
            }

            auto pos = mData.begin() + index;

            mData.remove(pos);

            if (index < mData.size()) {
                const Key& lastKey = mData[index].key;
                *getHashTableSlot(lastKey) = index;
            }
        }

        constexpr void clear() {
            mData.clear();
            std::ranges::fill(mHashTable, EMPTY_SLOT);
        }

        constexpr size_t size() const {
            return mData.size();
        }

        struct Iterator {
            // will fix (or not)
            using iterator_category = std::random_access_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            using value_type = Object;
            using pointer    = Object*;
            using reference  = Object&;

            constexpr Iterator(const pointer data) : data(data) {}

            constexpr reference operator*() {
                return *data;
            }

            constexpr pointer operator->() {
                return data;
            }

            constexpr reference operator[](size_t index) {
                return data[index];
            }

            constexpr Iterator& operator++() {
                ++data;
                return *this;
            }

            constexpr Iterator operator++(int) {
                Iterator temp = *this;
                ++data;
                return temp;
            }

            constexpr Iterator& operator--() {
                --data;
                return *this;
            }

            constexpr Iterator operator--(int) {
                Iterator temp = *this;
                --data;
                return temp;
            }

            constexpr auto operator<=>(const Iterator&) const = default;

        private:
             Object* data;
        };

        constexpr Iterator begin() noexcept {return {mData.begin()};}
        constexpr Iterator end() noexcept {return {mData.end()};}
    };

} // ggm
