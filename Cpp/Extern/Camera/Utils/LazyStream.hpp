//
// Created by Stefan on 01.05.2026.
//

#pragma once
#include <optional>
#include <vector>
#include <functional>
#include <cstdint>

namespace ggm {
    using u64 = unsigned long long;
    template<typename Function, typename ReturnType, typename... Args>
    concept returns = std::same_as<std::invoke_result_t<Function&, Args...>, ReturnType>;

    template<typename Filter, typename Type>
    concept isFilterFor = std::invocable<Filter&, const Type&> && returns<Filter, bool, const Type&>;

    template<typename Mapper, typename Type>
    concept isMapperFor = std::invocable<Mapper&, const Type&> && !returns<Mapper, void, const Type&>;

    template<typename Consumer, typename Type>
    concept isConsumerFor = std::invocable<Consumer&, const Type&> && returns<Consumer, void, const Type&>;

    template<typename F>
        struct Filter {
        F fn;
    };

    template<typename F>
    struct Mapper {
        F fn;
    };

    template<typename Container, u64 TAKE_LIMIT = SIZE_MAX, typename... Ops>
    requires requires (Container a) {a.begin(); a.end();}
    class LazyStream {
        const Container& mContainer;
        const std::tuple<Ops...> mOps;

        template<typename F, typename Input>
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

        using InitialType = std::iterator_traits<decltype(std::begin(mContainer))>::value_type;
        using FinalType = PipelineResult<InitialType, Ops...>::type;

        /// @brief Applies the accumulated functions to the value and returns an empty optional if filtered out
        template<u64 I, typename T, typename Tuple> constexpr
        auto process(const T& value, Tuple& ops) -> std::optional<FinalType> {
            if constexpr (I >= std::tuple_size_v<std::remove_reference_t<Tuple>>) {
                // End of function stack
                static_assert(std::is_same_v<T, FinalType>, "Type mismatch at pipeline end");
                return std::optional<FinalType>{value};
            } else {
                if constexpr (auto& op = std::get<I>(ops); std::is_same_v<std::decay_t<decltype(op)>, Filter<decltype(op.fn)>>) {
                    // Filter
                    if (!op.fn(value)) return std::optional<FinalType>{std::nullopt};
                    return process<I + 1, T>(value, ops);
                } else {
                    // Mapper
                    auto result = op.fn(value);
                    return process<I + 1, decltype(result)>(result, ops);
                }
            }
        }

    public:
        constexpr explicit LazyStream(const Container& container) :
            mContainer(container),
            mOps()
        {}

        constexpr LazyStream(const Container& container, std::tuple<Ops...> o) :
            mContainer(container),
            mOps(std::move(o))
        {}

        constexpr auto toVector() -> std::vector<FinalType> {
            std::vector<FinalType> out{};
            out.reserve(mContainer.size());

            size_t taken = 0;
            for (const auto& thing: mContainer) {
                if constexpr (TAKE_LIMIT < SIZE_MAX) if (taken >= TAKE_LIMIT) break;
                if (std::optional<FinalType> value = process<0, InitialType>(thing, mOps)) {
                    out.push_back(std::move(*value));
                    if constexpr (TAKE_LIMIT < SIZE_MAX) ++taken;
                }
            }
            return out;
        }

        constexpr auto toArray() -> std::array<FinalType, TAKE_LIMIT> {
            static_assert(TAKE_LIMIT != SIZE_MAX, "Array size is not known. Use take() first or consider toVector()");
            std::array<FinalType, TAKE_LIMIT> out;

            u64 taken = 0;
            for (const auto& thing: mContainer) {
                if (taken >= TAKE_LIMIT) break;
                if (std::optional<FinalType> value = process<0, InitialType>(thing, mOps)) {
                    out[taken] = std::move(*value);
                    ++taken;
                }
            }
            return out;
        }

        constexpr auto getFirst() -> FinalType {
            for (const auto& thing: mContainer) {
                std::optional<FinalType> value = process<0, InitialType>(thing, mOps);
                if (value) return *value;
            }
            throw std::runtime_error("Stream::getFirst() on empty result");
        }

        template<typename Consumer>
        requires isConsumerFor<Consumer, FinalType>
        constexpr void forEach(Consumer&& c) {
            u64 taken = 0;
            for (const auto& thing: mContainer) {
                if constexpr (TAKE_LIMIT < SIZE_MAX) if (taken >= TAKE_LIMIT) break;
                if (std::optional<FinalType> value = process<0, InitialType>(thing, mOps)) {
                    std::invoke(c, value);
                    if constexpr (TAKE_LIMIT < SIZE_MAX) ++taken;
                }
            }
        }

        template<typename Map>
        requires isMapperFor<Map, FinalType>
        constexpr auto map(Map&& m) {
            using MOp = Mapper<Map>;

            auto newOps = std::tuple_cat(
                mOps,
                std::tuple{MOp{std::forward<Map>(m)}}
            );

            return LazyStream<Container, TAKE_LIMIT, Ops..., MOp>(mContainer, newOps);
        }

        template<typename Filt>
        requires isFilterFor<Filt, FinalType>
        constexpr auto filter(Filt&& f) {
            using FOp = Filter<Filt>;

            auto newOps = std::tuple_cat(
                mOps,
                std::tuple{FOp{std::forward<Filt>(f)}}
            );

            return LazyStream<Container, TAKE_LIMIT, Ops..., FOp>(mContainer, newOps);
        }

        template<u64 n>
        constexpr auto take() const {
            return LazyStream<Container, n, Ops...>(mContainer, mOps);
        }
    };
} // ggm
