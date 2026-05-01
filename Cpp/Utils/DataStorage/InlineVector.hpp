//
// Created by ertls on 23.02.2026.
//

#pragma once

#include <cassert>
#include <stdexcept>

#include "../Math/ggmdef.hpp"

namespace ggm {
    template <typename T, u64 InlineCapacity = 0>
    class InlineVector {
    public:
        explicit InlineVector(u64 capacity = InlineCapacity) {
            if constexpr (InlineCapacity == 0) {
                mData = static_cast<T *>(operator new(capacity * sizeof(T)));
                this->mCapacity = capacity;
                mIsHeap = true;
            } else {
                if (capacity <= InlineCapacity) {
                    mData = inlinePtr();
                    this->mCapacity = InlineCapacity;
                    mIsHeap = false;
                } else {
                    mData = static_cast<T *>(operator new(capacity * sizeof(T)));
                    this->mCapacity = capacity;
                    mIsHeap = true;
                }
            }
            mSize = 0;
        }

        InlineVector(InlineVector&& other)  noexcept {
            if constexpr (InlineCapacity == 0) {
                mData = other.mData;
                mSize = other.mSize;
                mCapacity = other.mCapacity;

                other.mData = nullptr;
                other.mSize = 0;
                other.mCapacity = 0;
            } else {
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
        }

        InlineVector(const InlineVector& other) {
            if constexpr (InlineCapacity == 0) {
                mData = static_cast<T*>(operator new(other.mCapacity * sizeof(T)));
                for (u64 i = 0; i < other.mSize; ++i) {
                    std::construct_at(&mData[i], other.mData[i]);
                }
                mCapacity = other.mCapacity;
                mSize = other.mSize;
            } else {
                if (other.mIsHeap) {
                    mData = static_cast<T*>(operator new(other.mCapacity * sizeof(T)));
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
        }

        ~InlineVector() {
            if constexpr (InlineCapacity == 0) {
                operator delete(mData);
            } else {
                for (u64 i = 0; i < mSize; ++i)
                    std::destroy_at(&mData[i]);
                if (mIsHeap) operator delete(mData);
            }
        }

        InlineVector& operator=(InlineVector&& other)  noexcept {
            if constexpr (InlineCapacity == 0) {
                mData = other.mData;
                mCapacity = other.mCapacity;
                mSize = other.mSize;

                other.mData = nullptr;
                other.mCapacity = 0;
                other.mSize = 0;

                return *this;
            } else {
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

        template <class U>
        InlineVector& operator<<(U&& thing) {
            add(std::forward<U>(thing));
            return *this;
        }

        T& front() {
            return mData[0];
        }

        T& back() {
            return mData[mSize-1];
        }

        void growTo(u64 capacity) {
            if (capacity <= mCapacity) return;
            grow(capacity);
        }

        T& operator[](u64 index) noexcept {
            assert(index < mCapacity);
            return mData[index];
        }

        T& get(u64 index) {
            if (index >= mSize) throw std::out_of_range("Index out of bounds in ArrayList");
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
            static_assert(InlineCapacity > 0);
            return std::launder(reinterpret_cast<T*>(_inlineRaw_));
        }

    public:
        using Iterator = T*;
        using const_iterator = const T*;

        Iterator begin() noexcept {return mData;}
        Iterator end() noexcept {return mData + mSize;}

        const_iterator begin() const noexcept {return mData;}
        const_iterator end() const noexcept {return mData + mSize;}

        const_iterator cbegin() noexcept {return mData;}
        const_iterator cend() noexcept {return mData + mSize;}


        using reverse_iterator = std::reverse_iterator<Iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;


        reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        reverse_iterator rend()   noexcept { return reverse_iterator(begin()); }


        const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator rend()   const noexcept { return const_reverse_iterator(begin()); }

        const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }
        const_reverse_iterator crend()   const noexcept { return const_reverse_iterator(begin()); }

        Iterator erase_fast(Iterator first, Iterator last) {
            if (first == last) return first;
            auto newEnd = end()- (last - first);

            std::move(newEnd, end(), first);

            for (auto curr = newEnd; curr != end(); ++curr) {
                std::destroy_at(&(*curr));
            }
            mSize -= last-first;

            return first;
        }

        Iterator erase_fast(Iterator pos) {
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
