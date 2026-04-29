//
// Created by ertls on 27.04.2026.
//

#pragma once
#include <functional>

#include "InlineVector.hpp"


// TODO Make them lazy
namespace ggm {
    template<typename T>
    class Stream {
        InlineVector<T> mData{};

        template<typename S>
        class StreamBuilder {
            InlineVector<S> mData{16};
        public:
            StreamBuilder() = default;

            template<typename U>
            void append(U&& thing) {
                mData.add(std::forward<U>(thing));
            }

            Stream<S> toStream() {
                return Stream<S>{std::move(mData)};
            }
        };
    public:
        Stream()= default;
        explicit Stream(InlineVector<T>&& data) : mData(std::move(data))
        {}
        template<std::ranges::input_range R>
        requires (!std::same_as<std::remove_cvref_t<R>, Stream>) && std::constructible_from<T, std::ranges::range_reference_t<R>>
        explicit Stream(R&& range) {
            for (auto&& v : range) {
                mData.add(T(std::forward<decltype(v)>(v)));
            }
        }

        ~Stream() = default;

        std::vector<T> toVector() {
            std::vector<T> out;
            out.reserve(mData.size());

            std::move(mData.begin(), mData.end(), std::back_inserter(out));

            return out;
        }

        InlineVector<T> toInlineVector() {
            return std::move(mData);
        }

        template<typename Mapper>
        requires std::invocable<Mapper&, const T&> && (!std::same_as<std::invoke_result_t<Mapper&, const T&>, void>)
        auto map(Mapper&& m) {
            using ResultType =  std::remove_cvref_t<std::invoke_result_t<Mapper&&, const T&>>;
            StreamBuilder<ResultType> result{};

            for (auto& element : mData) {
                result.append(std::invoke(m, element));
            }

            return result.toStream();
        }

        template<typename Filter>
        requires std::predicate<Filter&, const T&>
        Stream& filter(Filter&& f) {
            for (size_t i = 0; i < mData.size();) {
                if (!std::invoke(f, mData[i])) mData.erase(mData.begin() + i);
                else ++i;
            }
            return *this;
        }

        template<typename Consumer>
        requires std::invocable<Consumer&, const T&> && std::same_as<std::invoke_result_t<Consumer&, const T&>, void>
        void forEach(Consumer&& c) {
            for (auto& d: mData) {
                std::invoke(c, d);
            }
        }

        template<typename Mapper>
        requires std::invocable<Mapper&, T&> && std::same_as<std::invoke_result_t<Mapper&, T&>, void>
        Stream& transform(Mapper&& m) {
            for (auto& x : mData) {
                std::invoke(m, x);
            }
            return *this;
        }

        template<typename Observer>
        requires std::invocable<Observer&, const T&> && std::same_as<std::invoke_result_t<Observer&, const T&>, void>
        Stream& peek(Observer&& o) const {
            for (auto& x : mData) {
                std::invoke(o, x);
            }
            return *this;
        }


        size_t count() {
            return mData.size();
        }
    };
}
