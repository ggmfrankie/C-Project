//
// Created by ertls on 28.07.2026.
//

#pragma once
#include <concepts>
#include <json.hpp>

#include "LazyStream.hpp"
#include "Parsing.hpp"
#include "Utils.hpp"

#define MCP_CAT2(a,b) a##b
#define MCP_CAT(a,b) MCP_CAT2(a,b)

class McpInterface {
    using Json = nlohmann::basic_json<>;
    using string = std::string;

    McpInterface() {
        mRequestableFunctions.reserve(16);
    }

    struct NameMap {
        const std::string_view type;
        const std::string_view jsonName;
    };

    static constexpr std::string_view getJsonName(const std::string_view cppTypeName) {
        constexpr std::array<NameMap, 8> jsonNames {
            NameMap{.type = "int", .jsonName = "integer"},
            NameMap{.type = "bool", .jsonName = "boolean"},
            NameMap{.type = "std::basic_string", .jsonName = "string"}
        };

        for (auto[type, jsonName]: jsonNames) {
            if (cppTypeName.starts_with(type)) return jsonName;
        }
        return "Invalid";
    }

    template<typename F, typename... Args>
    requires std::invocable<F, Args...>
    static Json _newRequest(int id, const string&& name, F&& func, Args&&... args) {
        constexpr std::array<string, sizeof...(Args)> argTypes = {typeid(std::remove_cvref_t<Args>).name()...};

        Json out;
        out["jsonrpc"] = "2.0";
        out["id"] = id;
        out["method"] = name;

        if constexpr (argTypes.empty()) return out;
        for (const auto& argName: argTypes) {
            out["result"] += {getJsonName(argName), };
        }

    }

    template<typename F, typename... Args>
    requires std::invocable<F, Args...>
    static Json invokeMethod(F&& method, Args&&... args) {
        using Result = std::invoke_result_t<F, Args...>;

        Result result = std::invoke(
            std::forward<F>(method),
            std::forward<Args>(args)...
        );

        Json out = {
            {"jsonrpc", "2.0"},
            {"id", 4},
            {
                "result",
                {"content",
                    {}
                }
            }
        };

        if constexpr(std::same_as<Result, bool>){

            out["result"]["content"] = {{
                {"type", "bool"},
                {"bool", result}
            }};

        }
        return out;
    }

    std::unordered_map<string, ggm::Parsing::Function> mRequestableFunctions{};

public:
    static McpInterface& Get() {
        static McpInterface INTERFACE{};
        return INTERFACE;
    }

    bool _processRequestableFunction(const std::string_view signature) {
        const auto functionParameter = ggm::Parsing::extractParameters(signature);
        mRequestableFunctions[functionParameter.name] = functionParameter;
        return false;
    }

#define MakeRequestableFunction(...)\
    inline static const bool MCP_CAT(_mcp_reg_, __COUNTER__) = \
        McpInterface::Get()._processRequestableFunction(#__VA_ARGS__);\
        __VA_ARGS__

};
