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
            Key key;
            Value value;
        };

        ArrayList<i64> mHashTable;
        ArrayList<Object> mData;
        Hash mHash {};

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

        using Iterator = decltype(mData)::Iterator;

        Iterator begin() noexcept {return mData.begin();}
        Iterator end() noexcept {return mData.end();}

    };

} // ggm
