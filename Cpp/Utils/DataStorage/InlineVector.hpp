//
// Created by ertls on 23.02.2026.
//

#ifndef MIXEDPROJECT_ARRAYLIST_H
#define MIXEDPROJECT_ARRAYLIST_H

#include <cstdlib>
#include <iostream>

#include "../Math/ggmdef.hpp"

namespace ggm {
    template <typename T, u64 InlineCapacity>
    class InlineVector {
    public:
        explicit InlineVector(u64 capacity = InlineCapacity) {
            if (capacity <= InlineCapacity) {
                mData = inlinePtr();
                this->mCapacity = InlineCapacity;
                mIsHeap = false;
            } else {
                mData = static_cast<T *>(operator new(capacity * sizeof(T)));
                this->mCapacity = capacity;
                mIsHeap = true;
            }
            mSize = 0;
        }

        InlineVector(InlineVector&& other)  noexcept {
            if (other.mIsHeap) {
                mData = other.mData;
                other.mIsHeap = false;
                mIsHeap = true;
            } else {
                mData = inlinePtr();
                for (u64 i = 0; i < other.mSize; ++i) {
                    std::construct_at(&mData[i], std::move(other.mData[i]));
                    std::destroy_at(&other.mData[i]);
                }
                mIsHeap = false;
            }
            mCapacity = other.mCapacity;
            mSize = other.mSize;

            other.mData = other.inlinePtr();
            other.mCapacity = InlineCapacity;
            other.mSize = 0;
        }

        InlineVector(const InlineVector& other) {
            if (other.mIsHeap) {
                mData = static_cast<T *>(operator new(other.mCapacity * sizeof(T)));
                mIsHeap = true;
            } else {
                mData = inlinePtr();
                mIsHeap = false;
            }
            for (u64 i = 0; i < other.mSize; ++i) {
                std::construct_at(&mData[i], other.mData[i]);
            }
            mCapacity = other.mCapacity;
            mSize = other.mSize;
        }

        ~InlineVector() {
            for (u64 i = 0; i < mSize; ++i)
                std::destroy_at(&mData[i]);
            if (mIsHeap) operator delete(mData);
        }

        InlineVector& operator=(InlineVector&& other)  noexcept {
            if (other.mIsHeap) {
                mData = other.mData;
                other.mIsHeap = false;
                mIsHeap = true;
            } else {
                mData = inlinePtr();
                for (u64 i = 0; i < other.mSize; ++i) {
                    std::construct_at(&mData[i], std::move(other.mData[i]));
                    std::destroy_at(&other.mData[i]);
                }
                mIsHeap = false;
            }
            mCapacity = other.mCapacity;
            mSize = other.mSize;

            other.mData = other.inlinePtr();
            other.mCapacity = InlineCapacity;
            other.mSize = 0;
            return *this;
        }

        void add(const T& thing) {
            if (mCapacity <= mSize) grow(mCapacity*2+1);
            std::construct_at(&mData[mSize++], thing);
        }

        void add(T&& thing) {
            if (mCapacity <= mSize) grow(mCapacity*2+1);
            std::construct_at(&mData[mSize++], std::move(thing));
        }

        template<typename... Args>
        T& push(Args&&... args) {
            if (mSize == mCapacity) grow(mCapacity*2+1);
            T* ptr = std::construct_at(&mData[mSize], std::forward<Args>(args)...);
            ++mSize;
            return *ptr;
        }

        template <class U>
        InlineVector& operator<<(U&& thing) {
            add(std::forward<U>(thing));
            return *this;
        }

        void growTo(u64 capacity) {
            if (capacity <= mCapacity) return;
            grow(capacity);
        }

        T& operator[](u64 index) noexcept {
            if (index >= mSize) {
                std::cerr << "index out of bounds in ArrayList\n";
                exit(-1);
            }
            return mData[index];
        }

        T& get(u64 index) {
            if (index >= mSize) {
                std::cerr << "index out of bounds in ArrayList\n";
                exit(-1);
            }
            return mData[index];
        }

        void clear() noexcept {
            for (u64 i = 0; i < mSize; ++i) {
                std::destroy_at(&mData[i]);
            }
            mSize = 0;
        }

        void fit() {
            if (!mIsHeap || mSize == mCapacity) return;

            T* old = mData;
            mData = static_cast<T*>(operator new(mSize * sizeof(T)));
            mCapacity = mSize;

            for (u64 i = 0; i < mSize; ++i) {
                std::construct_at(&mData[i], std::move(old[i]));
                std::destroy_at(&old[i]);
            }
            operator delete(old);
        }

        const T* data() const noexcept {
            return mData;
        }

        [[nodiscard]] bool empty() const noexcept {
            return mSize == 0;
        }

        [[nodiscard]] u64 size() const noexcept {
            return mSize;
        }

        [[nodiscard]] u64 capacity() const noexcept {
            return mCapacity;
        }

    private:
        bool mIsHeap = false;
        T* mData{};
        u64 mSize = 0;
        u64 mCapacity = 0;
        alignas(T) std::byte _inlineRaw_[sizeof(T) * InlineCapacity]{};

        void grow(u64 capacity) {
            T* oldData = mData;
            mCapacity = capacity;
            mData = static_cast<T *>(operator new(mCapacity * sizeof(T)));

            for (u64 i = 0; i < mSize; i++) {
                std::construct_at(&mData[i], std::move(oldData[i]));
                std::destroy_at(&oldData[i]);
            }
            if (mIsHeap) {
                operator delete(oldData);
            }
            mIsHeap = true;
        }

        T* inlinePtr() noexcept {
            return std::launder(reinterpret_cast<T*>(_inlineRaw_));
        }

    public:
        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept {return mData;}
        iterator end() noexcept {return mData + mSize;}

        const_iterator begin() const noexcept {return mData;}
        const_iterator end() const noexcept {return mData + mSize;}

        const_iterator cbegin() noexcept {return mData;}
        const_iterator cend() noexcept {return mData + mSize;}


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

#endif //MIXEDPROJECT_ARRAYLIST_H