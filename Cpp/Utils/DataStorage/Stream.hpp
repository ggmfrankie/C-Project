//
// Created by ertls on 27.04.2026.
//

#pragma once
#include "InlineVector.hpp"

namespace ggm {
    template<typename T>
    class Stream {
        InlineVector<T> mData;

        template<typename S>
        class StreamBuilder {
            InlineVector<T> mData;
        public:
            StreamBuilder() = default;


            void append(T&& thing) {
                mData.add(std::move(thing));
            }
        };
    public:
        Stream()= default;
        explicit Stream(InlineVector<T>&& data) : mData(std::move(data))
        {}
        ~Stream() = default;

        template<typename Mapper>
        requires std::invocable<Mapper&, const T&> && (!std::same_as<std::invoke_result_t<Mapper&, const T&>, void>)
        auto map(Mapper&& m) {

        }
    };
}
