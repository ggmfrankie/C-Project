//
// Created by Stefan on 22.03.2026.
//

#pragma once
#include <cstdlib>
#include <iostream>
#include <vector>

#include "Utils/DataStorage/InlineVector.hpp"
#include "Utils/Math/ggmdef.hpp"

namespace ggm {
    template <typename T>
    class SparseSet {
        InlineVector<T, 16> mDense;
        InlineVector<u64, 16> mSparse;
        InlineVector<u64, 16> mDenseToSparse;

        u64 updateIndices() {
            if (mSparse.size() < mDense.size()) {
                mDenseToSparse.growTo(mDense.capacity());
                mSparse.growTo(mDense.capacity());

                auto idx = mDense.size()-1;
                mSparse.add(idx);
                //mDenseToSparse[idx] = mSparse.size()-1;
            }
            return mSparse.size()-1;
        }

    public:

        explicit SparseSet(u64 capacity) :
            mDense(capacity),
            mSparse(capacity),
            mDenseToSparse(capacity)
        {
        }

        u64 add(const T& thing) {
            mDense.add(thing);
            return updateIndices();
        }

        u64 add(T&& thing) {
            mDense.add(std::move(thing));
            return updateIndices();
        }

        template<typename... Args>
        u64 push(Args&&... args) {
            mDense.push(std::forward<Args>(args)...);
            return updateIndices();
        }

        T& get(u64 index) {
            if (index >= mDense.size()) {
                std::cerr << "index: " << index <<" out of bounds in SparseSet\n";
                exit(-3);
            }
            return mDense[mSparse[index]];
        }

        T& operator[](u64 index) {
            return mDense[mSparse[index]];
        }

        [[nodiscard]] u64 size() const {
            return mDense.size();
        }

        [[nodiscard]] u64 capacity() const {
            return mDense.capacity();
        }

        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept {return mDense.begin();}
        iterator end() noexcept {return mDense.end();}

        const_iterator begin() const noexcept {return mDense.begin();}
        const_iterator end() const noexcept {return mDense.end();}

        const_iterator cbegin() noexcept {return mDense.cbegin();}
        const_iterator cend() noexcept {return mDense.cend();}



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