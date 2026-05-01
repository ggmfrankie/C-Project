//
// Created by Stefan on 01.05.2026.
//

#pragma once
#include <optional>
#include <vector>

#include "InlineVector.hpp"

namespace ggm {
    template<typename F>
        struct Filter {
        F fn;
        static constexpr bool IS_FILTER = true;
    };

    template<typename F>
    struct Mapper {
        F fn;
    };
    template<typename Container, typename... Ops>
    class LazyStream {
        const Container& mContainer;
        std::tuple<Ops...> mOps;

        template<typename Op, typename Input>
        struct GetType;

        template<typename F, typename Input>
        struct GetType<Filter<F>, Input> {
            using type = Input;
        };

        template<typename F, typename Input>
        struct GetType<Mapper<F>, Input> {
            using type = std::invoke_result_t<F, Input>;
        };

        template<typename Input, typename... Ops_p>
        struct PipelineResult;

        template<typename Input>
        struct PipelineResult<Input> {
            using type = Input;
        };

        template<typename Input, typename Op, typename... Rest>
        struct PipelineResult<Input, Op, Rest...> {
            using Next = GetType<Op, Input>::type;
            using type = PipelineResult<Next, Rest...>::type;
        };

        using DataType = std::iterator_traits<decltype(std::begin(mContainer))>::value_type;
        using FinalT = PipelineResult<DataType, Ops...>::type;

        template<std::size_t I, typename T, typename Tuple>
        auto process(const T& value, Tuple& ops) {
            if constexpr (I >= std::tuple_size_v<std::remove_reference_t<Tuple>>) {
                return std::optional<FinalT>{value};
            } else {
                auto& op = std::get<I>(ops);
                if constexpr (requires {decltype(op)::IS_FILTER; }) {
                    // Filter
                    if (!op.fn(value)) return std::optional<FinalT>{std::nullopt};
                    return process<I + 1, T, Tuple>(value, ops);
                } else {
                    // Mapper
                    auto result = op.fn(value);
                    return process<I + 1, decltype(result), Tuple>(result, ops);
                }
            }
        }

    public:
        explicit LazyStream(const Container& container) : mContainer(container), mOps(){}

        LazyStream(const Container& container, std::tuple<Ops...> o) : mContainer(container), mOps(std::move(o)){}

        auto toVector() {

            std::vector<FinalT> out{};
            out.reserve(mContainer.size());

            for (const auto& thing: mContainer) {
                std::optional<FinalT> value = process<0, DataType, std::tuple<Ops...>>(thing, mOps);
                if (value) out.push_back(std::move(*value));
            }

            return out;
        }

        template<typename Map>
        auto map(Map&& m) {
            using MOp = Mapper<std::decay_t<Map>>;

            auto newOps = std::tuple_cat(
                mOps,
                std::tuple{MOp{std::forward<Map>(m)}}
            );

            return LazyStream<Container, Ops..., MOp>(mContainer, newOps);
        }

        template<typename Filt>
        auto filter(Filt&& f) {
            using FOp = Filter<std::decay_t<Filt>>;

            auto newOps = std::tuple_cat(
                mOps,
                std::tuple{FOp{std::forward<Filt>(f)}}
            );

            return LazyStream<Container, Ops..., FOp>(mContainer, newOps);
        }

    };
} // ggm
