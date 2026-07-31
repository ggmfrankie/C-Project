//
// Created by ertls on 28.07.2026.
//

#pragma once
#include <concepts>
#include <json.hpp>
#include <memory>
#include <stdexcept>

#include "LazyStream.hpp"
#include "Parsing.hpp"
#include "Utils.hpp"

#define MCP_CAT(a,b) a##b

class McpFunctionRegistry {
    using Json = nlohmann::basic_json<>;
    using string = std::string;

    McpFunctionRegistry() {
        mRequestableFunctions.reserve(16);
    }

    struct NameMap {
        const std::string_view type;
        const std::string_view jsonName;
    };

    static constexpr std::string_view getJsonName(const std::string_view cppTypeName) {
        constexpr std::array jsonNames {
            NameMap{.type = "int", .jsonName = "integer"},
            NameMap{.type = "bool", .jsonName = "boolean"},
            NameMap{.type = "std::basic_string", .jsonName = "string"}
        };

        for (auto[type, jsonName]: jsonNames) {
            if (cppTypeName.starts_with(type)) return jsonName;
        }
        return "Invalid";
    }

    struct Function {
        using Metadata = ggm::Parsing::Function;

        virtual ~Function() = default;
        virtual Metadata& getMetadata();
        virtual Json operator()(const Json& args) = 0;
    };

    template<typename F>
    struct FunctionWrapper {
        explicit FunctionWrapper(F&& function):
            mFunction(function)
        {}

        template<typename... Args, std::size_t... I>
        static std::tuple<Args...> argsFromJsonArrayImpl(const Json& params, std::index_sequence<I...>) {
            return { params.at(I).get<Args>()... };
        }

        template<typename... Args>
        std::tuple<Args...> argsFromJson(const Json& params) {
            if (!params.is_array() || params.size() != sizeof...(Args)) {
                throw std::runtime_error("Invalid params");
            }
            return argsFromJsonArrayImpl<Args...>(params, std::index_sequence_for<Args...>{});
        }

        template<typename... Args>
        requires std::invocable<F, Args...>
        Json invokeFunction(F&& function, Args&&... args) {
            using Result = std::invoke_result_t<F, Args...>;

            Result result = std::invoke(
                std::forward<F>(function),
                std::forward<Args>(args)...
            );

            Json out;
            if constexpr (!std::is_same_v<Result, void>) {
                out = {
                    {"type", getJsonName(mMetaData.returnType)},
                    {mMetaData.returnType, result}
                };
            } else if constexpr (std::is_same_v<Result, Json>){
                out = result;
            }

            return out;
        }

        ggm::Parsing::Function mMetaData;
        F mFunction;

        Json operator()(const Json& args) {
            return invokeMethod(mFunction, argsFromJson<>(args));
        }
    };

    std::unordered_map<string, std::unique_ptr<Function>> mRequestableFunctions{};

public:
    static McpFunctionRegistry& Get() {
        static McpFunctionRegistry INTERFACE{};
        return INTERFACE;
    }

    Json getAllFunctions() {
        Json out;
        for (auto &val: mRequestableFunctions | std::views::values) {
            auto& function = *val;
            auto& meta = function.getMetadata();
            out["name"] = meta.name;
            out["inputSchema"] = {"type", "object"};
            for (const auto&[type, name]: meta.parameters) {
                out["inputSchema"]["properties"][name] = {
                    {"type", getJsonName(type)}
                };
            }
        }
    }

    template<typename F>
    bool registerFunction(const std::string_view signature, F&& function) {
        const auto functionParameter = ggm::Parsing::extractParameters(signature);

        mRequestableFunctions[functionParameter.name] = FunctionWrapper<F>{function};
        return false;
    }

#define MakeRequestableFunction(...)\
    inline static const bool MCP_CAT(_mcp_registrated_, __COUNTER__) = \
        McpFunctionRegistry::Get().registerFunction(#__VA_ARGS__);\
        __VA_ARGS__

};
