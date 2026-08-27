//
// Created by Stefan on 22.03.2026.
//

#pragma once

#include <sstream>

#include "Utils/DataStorage/InlineVector.hpp"
#include "../ggmdef.hpp"

namespace ggm {
    template <typename T>
    class SparseSet {
        InlineVector<T> mData;
        InlineVector<u64> mIndices;
        InlineVector<u64> mReverseIndices;

        u64 updateIndices() {
            if (mIndices.size() < mData.size()) {
                mReverseIndices.growTo(mData.capacity());
                mIndices.growTo(mData.capacity());

                auto idx = mData.size()-1;
                mIndices.add(idx);
                //mReverseIndices[idx] = mData.size()-1;
            }
            return mIndices.size()-1;
        }

    public:
        explicit SparseSet(u64 capacity) :
            mData(capacity),
            mIndices(capacity),
            mReverseIndices(capacity)
        {
        }

        u64 add(const T& thing) {
            mData.add(thing);
            return updateIndices();
        }

        u64 add(T&& thing) {
            mData.add(std::move(thing));
            return updateIndices();
        }

        template<typename... Args>
        u64 push(Args&&... args) {
            mData.push(std::forward<Args>(args)...);
            return updateIndices();
        }

        T& get(u64 index) {
            if (index >= mData.size()) {
                std::ostringstream error;
                error << "Index: "<< index <<" out of bounds in SparseSet with size: "<< mData.size() <<"\n";
                throw std::out_of_range(error.str());
            }
            return mData[mIndices[index]];
        }

        T& operator[](u64 index) {
            return mData[mIndices[index]];
        }

        [[nodiscard]] u64
        size() const {
            return mData.size();
        }

        [[nodiscard]] u64
        capacity() const {
            return mData.capacity();
        }

        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept {return mData.begin();}
        iterator end() noexcept {return mData.end();}

        const_iterator begin() const noexcept {return mData.begin();}
        const_iterator end() const noexcept {return mData.end();}

        const_iterator cbegin() noexcept {return mData.cbegin();}
        const_iterator cend() noexcept {return mData.cend();}

        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        reverse_iterator rend()   noexcept { return reverse_iterator(begin()); }

        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator rend()   const noexcept { return const_reverse_iterator(begin()); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(begin()); }

    };
} // ggm