//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include "ArrayList.hpp"
#include <concepts>
#include <functional>
#include <optional>

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

        auto getHashTableSlot(const Key& key) {
            const size_t index = mHash(key) % mHashTable.capacity();

            const auto begin = mHashTable.begin();
            auto end = begin + mHashTable.capacity();

            auto it = begin + index;

            while (*it != EMPTY_SLOT) {
                if (mData[*it].key == key) return it;
                ++it;
                if (it == end) it = begin;
            }
            return end;
        }

        auto getEmptyHashTableSlot(const Key& key) {
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

        void rehash() {
            std::ranges::fill(mHashTable, EMPTY_SLOT);
            for (u64 i = 0; i < mData.size(); ++i) {
                auto&[key, value] = mData[i];

                if (const auto hashIndex = getEmptyHashTableSlot(key);
                    hashIndex != mHashTable.end()) {
                    *hashIndex = i;
                } else {
                    throw std::runtime_error("Rehashing failed");
                }
            }
        }

        void resizeHashTable(const u64 newCapacity) {
            mHashTable.growTo(newCapacity, -1);
            rehash();
        }

    public:
        explicit HashMap(const u64 capacity = 16) noexcept :
            mHashTable(capacity * HASH_TABLE_FACTOR, EMPTY_SLOT),
            mData(capacity)
        {}

        void insert(const Key& key, const Value& value) {
            // Check if hashtable needs resize
            if (static_cast<float>(mData.size()) / mHashTable.capacity() > 0.75) resizeHashTable(mHashTable.capacity() * 2);

            u64 index = mData.size();
            mData.push(key, value);

            *getEmptyHashTableSlot(key) = index;
        }

        bool contains(const Key& key) {
            return getHashTableSlot(key) != mHashTable.end();
        }

        Value* get(const Key& key) {
            const auto hashTableSlot = getHashTableSlot(key);
            return (hashTableSlot != mHashTable.end())
                ? &mData[*hashTableSlot].value
                : nullptr;
        }

        Value* operator[](const Key& key) {
            return get(key);
        }

        auto find(const Value& value) {
            return std::ranges::find(mData, value, &Object::value);
        }

        void remove(const Key& key) {
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

            if (mData.end() - pos > 1) {
                const Key& lastKey = mData[index].key;
                *getHashTableSlot(lastKey) = index;
            }
        }

        size_t size() {
            return mData.size();
        }

        struct Iterator {
            // will fix
            using iterator_category = std::random_access_iterator_tag;
            using difference_type   = std::ptrdiff_t;

            using value_type = Object;
            using pointer    = Object*;
            using reference  = Object&;

            Iterator(const pointer data) : data(data) {}

            reference operator*() {
                return *data;
            }

            pointer operator->() {
                return data;
            }

            reference operator[](size_t index) {
                return data[index];
            }

            Iterator& operator++() {
                ++data;
                return *this;
            }

            Iterator operator++(int) {
                Iterator temp = *this;
                ++data;
                return temp;
            }

            Iterator& operator--() {
                --data;
                return *this;
            }

            Iterator operator--(int) {
                Iterator temp = *this;
                --data;
                return temp;
            }

            auto operator<=>(const Iterator&) const = default;

        private:
             Object* data;
        };

        Iterator begin() noexcept {return {mData.begin()};}
        Iterator end() noexcept {return {mData.end()};}
    };

} // ggm
