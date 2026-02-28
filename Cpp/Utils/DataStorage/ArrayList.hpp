//
// Created by ertls on 23.02.2026.
//

#ifndef MIXEDPROJECT_ARRAYLIST_H
#define MIXEDPROJECT_ARRAYLIST_H

#include <cstdlib>
#include <iostream>

#include "../Math/ggmdef.hpp"

namespace Render {
    template <typename T, num::u64 InlineCapacity>
    class ArrayList {
    public:
        explicit ArrayList(num::u64 capacity = InlineCapacity) {
            if (capacity <= InlineCapacity) {
                data = inlinePtr();
                this->capacity = InlineCapacity;
                isHeap = false;
            } else {
                data = static_cast<T *>(operator new(capacity * sizeof(T)));
                this->capacity = capacity;
                isHeap = true;
            }
            size = 0;
        }

        ArrayList(ArrayList&& other)  noexcept {
            if (other.isHeap) {
                data = other.data;
                other.isHeap = false;
                isHeap = true;
            } else {
                data = inlinePtr();
                for (num::u64 i = 0; i < other.size; ++i) {
                    std::construct_at(&data[i], std::move(other.data[i]));
                    std::destroy_at(&other.data[i]);
                }
                isHeap = false;
            }
            capacity = other.capacity;
            size = other.size;

            other.data = other.inlinePtr();
            other.capacity = InlineCapacity;
            other.size = 0;
        }

        ArrayList(const ArrayList& other) {
            if (other.isHeap) {
                data = static_cast<T *>(operator new(other.capacity * sizeof(T)));
                isHeap = true;
            } else {
                data = inlinePtr();
                isHeap = false;
            }
            for (num::u64 i = 0; i < other.size; ++i) {
                std::construct_at(&data[i], other.data[i]);
            }
            capacity = other.capacity;
            size = other.size;
        }

        ~ArrayList() {
            for (num::u64 i = 0; i < size; ++i)
                std::destroy_at(&data[i]);
            if (isHeap) operator delete(data);
        }

        void add(const T& thing) {
            if (capacity <= size) grow();
            std::construct_at(&data[size++], thing);
        }

        void add(T&& thing) {
            if (capacity <= size) grow();
            std::construct_at(&data[size++], std::move(thing));
        }

        template<typename... Args>
        T& push(Args&&... args) {
            if (size == capacity) grow();
            T* ptr = std::construct_at(&data[size], std::forward<Args>(args)...);
            ++size;
            return *ptr;
        }

        template <class U>
        ArrayList& operator<<(U&& thing) {
            add(std::forward<U>(thing));
            return *this;
        }

        T& operator[](num::u64 index) noexcept {
            if (index >= size) {
                std::cerr << "index out of bounds in ArrayList\n";
                exit(-1);
            }
            return data[index];
        }

        T& at(num::u64 index) {
            if (index >= size) {
                std::cerr << "index out of bounds in ArrayList\n";
                exit(-1);
            }
            return data[index];
        }

        void clear() noexcept {
            for (num::u64 i = 0; i < size; ++i) {
                std::destroy_at(&data[i]);
            }
            size = 0;
        }

        void fit() {
            if (!isHeap || size == capacity) return;

            T* old = data;
            data = static_cast<T*>(operator new(size * sizeof(T)));
            capacity = size;

            for (num::u64 i = 0; i < size; ++i) {
                std::construct_at(&data[i], std::move(old[i]));
                std::destroy_at(&old[i]);
            }
            operator delete(old);
        }

        const T* getDataPtr() const noexcept {
            return data;
        }

        [[nodiscard]] bool empty() const noexcept {
            return size == 0;
        }
    private:
        bool isHeap = false;
        T* data{};
        num::u64 size = 0;
        num::u64 capacity = 0;
        alignas(T) std::byte inlineRaw_[sizeof(T) * InlineCapacity]{};

    private:
        void grow() {
            T* oldData = data;
            capacity = capacity*2+1;
            data = static_cast<T *>(operator new(capacity * sizeof(T)));

            for (num::u64 i = 0; i < size; i++) {
                std::construct_at(&data[i], std::move(oldData[i]));
                std::destroy_at(&oldData[i]);
            }
            if (isHeap) {
                operator delete(oldData);
            }
            isHeap = true;
        }

        T* inlinePtr() noexcept {
            return std::launder(reinterpret_cast<T*>(inlineRaw_));
        }

    public:
        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() noexcept {return data;}
        iterator end() noexcept {return data + size;}

        const_iterator begin() const noexcept {return data;}
        const_iterator end() const noexcept {return data + size;}

        iterator cbegin() noexcept {return data;}
        iterator cend() noexcept {return data + size;}


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