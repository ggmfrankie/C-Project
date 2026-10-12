//
// Created by Stefan on 11.10.2026.
//

#pragma once
#include "ArrayList.hpp"
#include <concepts>
#include <functional>

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
            const Key key;
            Value value;
        };

        ArrayList<i64> mHashTable;
        ArrayList<Object> mData;

        [[no_unique_address]] Hash mHash {};

        std::size_t mSize = 0;

        static constexpr i64 EMPTY_SLOT = -1;
        static constexpr int HASH_TABLE_FACTOR = 4;

        auto getHashTableSlot(const Key& key) {
            const size_t index = mHash(key) % mHashTable.capacity();

            auto it = mHashTable.begin() + index;
            while (*it != -1) {
                if (mData[*it].key == key) return it;
                ++it;
                if (it == mHashTable.end()) it = mHashTable.begin();
            }
            return mHashTable.end();
        }

        void rehash() {
            std::ranges::fill(mHashTable, EMPTY_SLOT);
            for (u64 i = 0; i < mData.size(); ++i) {
                auto&[key, value] = mData[i];

                if (const auto hashIndex = getHashTableSlot(key);
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
            mHashTable(capacity, -1),
            mData(capacity * HASH_TABLE_FACTOR)
        {}

        void insert(const Key& key, const Value& value) {
            // Check if hashtable needs resize
            if (static_cast<float>(mSize) / mHashTable.capacity() > 0.75) resizeHashTable(mHashTable.capacity());

            u64 index = mData.size();
            mData.push(key, value);

            *getHashTableSlot(key) = index;
        }

        Value* operator[](const Key& key) {
            const auto hashTableSlot = getHashTableSlot(key);
            return hashTableSlot
                ? &mData[*hashTableSlot].value
                : nullptr;
        }

        void remove(const Key& key) {
            auto slot = getHashTableSlot(key);
            if (slot == nullptr || *slot == EMPTY_SLOT) return;

            const ssize_t index = *slot;

            const auto start = mHashTable.begin();
            const auto end = mHashTable.end();

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
                auto foundSlot = getHashTableSlot(mData[i]);

                *foundSlot = dataIndex;
            }

            mData.remove(index);
        }

        struct Iterator {
            using iterator_category = std::contiguous_iterator_tag;
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
