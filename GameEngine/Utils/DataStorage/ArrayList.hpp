//
// Created by ertls on 23.02.2026.
//

#pragma once

#include <cassert>
#include <stdexcept>
#include <iterator>
#include <memory>
#include <new>
#include <cstddef>
#include <utility>

#include "../ggmdef.hpp"

namespace ggm {
    template <typename T, u64 InlineCapacity = 0>
    class ArrayList {
    public:
        explicit ArrayList(u64 capacity = InlineCapacity) noexcept
        {
            if constexpr (InlineCapacity == 0) {
                mData = static_cast<T *>(operator new(capacity * sizeof(T)));
                mCapacity = capacity;
                mIsHeap = true;
            } else {
                if (capacity <= InlineCapacity) {
                    mData = inlinePtr();
                    mCapacity = InlineCapacity;
                    mIsHeap = false;
                } else {
                    mData = static_cast<T *>(operator new(capacity * sizeof(T)));
                    mCapacity = capacity;
                    mIsHeap = true;
                }
            }
            mSize = 0;
        }

        ArrayList(u64 capacity, const T& value) noexcept :
            ArrayList(capacity)
        {
            std::uninitialized_fill_n(mData, capacity, value);
            mSize = capacity;
        }

        ArrayList(ArrayList&& other) noexcept {
            if constexpr (InlineCapacity == 0) {
                mData = std::exchange(other.mData, nullptr);
                mSize = std::exchange(other.mSize, 0);
                mCapacity = std::exchange(other.mCapacity, 0);

            } else {
                if (other.mIsHeap) {
                    mData = other.mData;
                } else {
                    mData = inlinePtr();

                    std::uninitialized_move_n(other.mData, other.mSize, mData);
                    std::destroy_n(other.mData, other.mSize);
                }

                mIsHeap = std::exchange(other.mIsHeap, false);
                mCapacity = std::exchange(other.mCapacity, InlineCapacity);
                mSize = std::exchange(other.mSize, 0);

                other.mData = other.inlinePtr();
            }
        }

        ArrayList(const ArrayList& other) noexcept {
            if constexpr (InlineCapacity == 0) {
                mData = static_cast<T*>(operator new(other.mCapacity * sizeof(T)));

                std::uninitialized_copy_n(other.mData, other.mSize, mData);

                mCapacity = other.mCapacity;
                mSize = other.mSize;
            } else {
                if (other.mIsHeap) {
                    mData = static_cast<T*>(operator new(other.mCapacity * sizeof(T)));
                } else {
                    mData = inlinePtr();
                }
                mCapacity = other.mCapacity;
                mIsHeap = other.mIsHeap;

                std::uninitialized_copy_n(other.mData, other.mSize, mData);
                mSize = other.mSize;
            }
        }

        ~ArrayList() noexcept {
            std::destroy_n(mData, mSize);
            if constexpr (InlineCapacity == 0) {
                operator delete(mData);
            } else {
                if (mIsHeap) operator delete(mData);
            }
        }

        ArrayList& operator=(ArrayList&& other) noexcept {
            if (this == &other) return *this;

            std::destroy_n(mData, mSize);

            if constexpr (InlineCapacity == 0) {
                operator delete(mData);
                mData = std::exchange(other.mData, nullptr);
                mSize = std::exchange(other.mSize, 0);
                mCapacity = std::exchange(other.mCapacity, 0);
            } else {
                if (mIsHeap) operator delete(mData);

                mIsHeap = std::exchange(other.mIsHeap, false);

                if (mIsHeap) {
                    mData = other.mData;
                } else {
                    mData = inlinePtr();
                    std::uninitialized_move_n(other.mData, other.mSize, mData);
                    std::destroy_n(other.mData, other.mSize);
                }

                mCapacity = std::exchange(other.mCapacity, InlineCapacity);
                mSize = std::exchange(other.mSize, 0);

                other.mData = other.inlinePtr();
            }
            return *this;
        }

        T& add(const T& thing) {
            if (mCapacity <= mSize) grow(mCapacity*2+1);
            T* ptr = std::construct_at(&mData[mSize], thing);
            ++mSize;
            return *ptr;
        }

        T& add(T&& thing) {
            if (mCapacity <= mSize) grow(mCapacity*2+1);
            T* ptr = std::construct_at(&mData[mSize], std::move(thing));
            ++mSize;
            return *ptr;
        }

        template<typename... Args>
        T& push(Args&&... args) {
            if (mSize == mCapacity) grow(mCapacity*2+1);
            T* ptr = std::construct_at(&mData[mSize], std::forward<Args>(args)...);
            ++mSize;
            return *ptr;
        }

        template<typename Self>
        auto& front(this Self&& self) {
            return self.mData[0];
        }

        template<typename Self>
        auto& back(this Self&& self) {
            return self.mData[self.mSize - 1];
        }

        void growTo(u64 capacity) {
            if (capacity <= mCapacity) return;
            grow(capacity);
        }

        void growTo(u64 capacity, const T& value) {
            growTo(capacity);
            for (u64 i = mSize; i < capacity; ++i) {
                std::construct_at(mData + i, value);
            }
            mSize = capacity;
        }

        template<typename Self>
        auto& operator[](this Self&& self, u64 index) noexcept {
            assert(index < self.mCapacity);
            return self.mData[index];
        }

        template<typename Self>
        auto& get(this Self&& self, u64 index) {
            if (index >= self.mSize) throw std::out_of_range("Index out of bounds in ArrayList");
            return self.mData[index];
        }

        void clear() noexcept {
            std::destroy_n(mData, mSize);
            mSize = 0;
        }

        void fit() {
            if (!mIsHeap || mSize == mCapacity) return;

            T* oldData = mData;
            mData = static_cast<T*>(operator new(mSize * sizeof(T)));
            mCapacity = mSize;

            std::uninitialized_move_n(oldData, mSize, mData);
            std::destroy_n(oldData, mSize);
            operator delete(oldData);
        }

        template<typename Self>
        auto* data(this Self&& self) noexcept {
            return self.mData;
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
        T* mData = nullptr;
        u64 mSize = 0;
        u64 mCapacity = 0;
        alignas(T) std::byte _inlineRaw_[sizeof(T) * InlineCapacity]{};

        void grow(u64 capacity) noexcept {
            T* oldData = mData;
            mCapacity = capacity;
            mData = static_cast<T *>(operator new(mCapacity * sizeof(T)));

            std::uninitialized_move_n(oldData, mSize, mData);
            std::destroy_n(oldData, mSize);

            if constexpr (InlineCapacity == 0) {
                operator delete(oldData);
            } else {
                if (mIsHeap) {
                    operator delete(oldData);
                }
                mIsHeap = true;
            }
        }

        T* inlinePtr() noexcept {
            static_assert(InlineCapacity > 0);
            return std::launder(reinterpret_cast<T*>(_inlineRaw_));
        }

    public:
        using Iterator = T*;
        using CIterator = const T*;

        Iterator begin() noexcept {return mData;}
        Iterator end() noexcept {return mData + mSize;}

        CIterator begin() const noexcept {return mData;}
        CIterator end() const noexcept {return mData + mSize;}

        CIterator cbegin() noexcept {return mData;}
        CIterator cend() noexcept {return mData + mSize;}

        using RIterator = std::reverse_iterator<Iterator>;
        using CRIterator = std::reverse_iterator<CIterator>;

        RIterator rbegin() noexcept { return RIterator(end()); }
        RIterator rend()   noexcept { return RIterator(begin()); }

        CRIterator rbegin() const noexcept { return CRIterator(end()); }
        CRIterator rend()   const noexcept { return CRIterator(begin()); }

        CRIterator crbegin() const noexcept { return CRIterator(end()); }
        CRIterator crend()   const noexcept { return CRIterator(begin()); }

        Iterator remove(Iterator first, Iterator last) {
            if (first == last) return first;
            auto newEnd = end() - (last - first);

            std::move(newEnd, end(), first);

            for (auto curr = newEnd; curr != end(); ++curr) {
                std::destroy_at(&(*curr));
            }
            mSize -= last-first;

            return first;
        }

        Iterator remove(Iterator pos) {
            auto last = end()-1;

            if (pos == last) {
                std::destroy_at(&(*last));
                --mSize;
                return end();
            }

            *pos = std::move(*last);
            std::destroy_at(&*last);
            --mSize;
            return pos;
        }

        Iterator erase(Iterator first, Iterator last) {
            if (first == last) return first;

            std::move(last, end(), first);
            auto newEnd = end()- (last - first);

            for (auto curr = newEnd; curr != end(); ++curr) {
                std::destroy_at(&(*curr));
            }
            mSize -= last-first;

            return first;
        }

        Iterator erase(Iterator pos) {
            auto last = end()-1;

            if (pos == last) {
                std::destroy_at(&(*last));
                --mSize;
                return end();
            }

            std::move(pos+1, end(), pos);
            std::destroy_at(&*last);
            --mSize;
            return pos;
        }
    };
} // ggm
