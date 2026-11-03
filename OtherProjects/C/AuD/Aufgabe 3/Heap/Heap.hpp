//
// Created by ggmfrankie on 10/29/26.
//

#pragma once
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <vector>
#include <sys/types.h>

template<typename T>
concept Comparable =
    requires (T a, T b){a == b; a < b; a > b; a <= b; a >= b;};

template<Comparable T>
class Heap {
    T* mData = nullptr;
    size_t mSize = 0;
    size_t mCapacity = 0;

    [[nodiscard]] ssize_t leftChild(const size_t idx) const {
        return (idx * 2 + 1) < mSize ?: -1 ;
    }

    [[nodiscard]] ssize_t rightChild(const size_t idx) const {
        return (idx * 2 + 2) < mSize ?: -1;
    }

    static size_t getParent(const size_t childIndex) {
        return (childIndex > 0) ? childIndex >> 2 : -1;
    }

    void localMaxify(size_t index) {
        struct Element {
            T& element;
            ssize_t index;
            bool operator<(const Element& other) const {
                return element < other.element;
            }
        };

        ssize_t left = leftChild(index);
        if (left == -1) return;

        ssize_t right = rightChild(index);
        if (right == -1) return;

        T& top = mData[index];

        if (Element largest = std::max({{top, index}, {mData[left], left}, {mData[right], right}});
            largest.index != index)
        {
            std::swap(top, largest.element);
            localMaxify(largest.index);
        }
    }

public:
    explicit Heap(const size_t capacity) :
        mData(static_cast<T *>(operator new(capacity * sizeof(T)))),
        mCapacity(capacity)
    {}

    Heap(std::initializer_list<T> list) :
        Heap(list.size())
    {
        std::ranges::copy(list, mData);
        mSize = list.size();
        maxify();
    }

    ~Heap() {
        operator delete(mData);
    }

    Heap(const Heap& other) :
        mData(operator new(other.mData)),
        mSize(other.mSize),
        mCapacity(other.mCapacity)
    {}

    Heap(Heap&& other) noexcept :
        mData(other.mData),
        mSize(other.mSize),
        mCapacity(other.mCapacity)
    {}

    void maxify() {
        const size_t half = mCapacity/2 + 1;

        for (size_t i = half; i --> 0;) {
            localMaxify(i);
        }
    }

    std::vector<T> sort() && {
        while (mSize--) {
            mData[mSize] = mData[0];
            localMaxify(0);
        }

        return std::vector<T>{std::move(begin()), end()};
    }

    T* begin() {
        return mData;
    }

    T* end() {
        return mData + mSize;
    }
};

