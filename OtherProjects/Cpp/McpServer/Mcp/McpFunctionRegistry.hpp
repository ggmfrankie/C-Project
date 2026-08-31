//
// Created by ertls on 28.07.2026.
//

#pragma once
#include <concepts>
#include <execution>
#include "../Dependencies/json/json.hpp"
#include <memory>
#include <stdexcept>
#include <utility>

#include "../Utils/Parsing.hpp"
#include "../Utils/Utils.hpp"

#define MCP_CAT_IMPL(a, b) a##b
#define MCP_CAT(a, b) MCP_CAT_IMPL(a, b)
#define MCP_UNIQUE_NAME(name) MCP_CAT(name, __COUNTER__)

namespace mcp {
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

    struct IFunction {
        using Metadata = ggm::Parsing::Function;

        virtual ~IFunction() = default;
        virtual const Metadata& getMetadata() = 0;
        virtual const string& getDescription() = 0;
        virtual Json operator()(const Json& args) = 0;
    };

    template<typename F>
    struct FunctionWrapper: public IFunction {

        explicit FunctionWrapper(F function, Metadata  metadata, const string& description):
            mMetaData(std::move(metadata)),
            mDescription(description),
            mFunction(function)
        {
            const auto idx = mMetaData.name.find_first_of("::");
           //mMetaData.name.replace(idx, 2, "__");
        }

        template<typename T> struct FunctionTraits;

        template<typename Ret, typename... Args>
        struct FunctionTraits<Ret(*)(Args...)> {
            using ArgsTuple = std::tuple<Args...>;
        };

        template<typename... Args>
        Json invokeWithTuple(const Json& args, std::tuple<Args...>) {
            auto tup = argsFromJson<Args...>(args);
            return std::apply([this](Args... a) {
                    return invokeFunction(mFunction, std::forward<Args>(a)...);
                }, 
                std::move(tup)
            );
        }

        template<typename... Args, std::size_t... I>
        static std::tuple<Args...> argsFromJsonArrayImpl(const Json& params, std::index_sequence<I...>) {
            return { params.at(I).get<Args>()... };
        }

        template<typename... Args, std::size_t... I>
        std::tuple<Args...> argsFromJsonObjectImpl(const Json& params, std::index_sequence<I...>) {
            return {
                params.at(std::string(mMetaData.parameters[I].name)).get<Args>()...
            };
        }

        template<typename... Args>
        std::tuple<Args...> argsFromJson(const Json& params) {
            if (mMetaData.parameters.size() != sizeof...(Args) || params.size() != sizeof...(Args)) {
                std::cerr << "isArray: " << params.is_array() << " size of params: " << params.size() << " size of args: " << sizeof...(Args) << "\n";
                throw std::runtime_error("Invalid params");
            }
            return argsFromJsonObjectImpl<Args...>(params, std::index_sequence_for<Args...>{});
        }

        template<typename... Args>
        requires std::invocable<F, Args...>
        Json invokeFunction(F function, Args&&... args) {
            using Result = std::invoke_result_t<F, Args...>;

            Result result = std::invoke(
                std::forward<F>(function),
                std::forward<Args>(args)...
            );

            Json out;
            if constexpr (std::is_same_v<Result, void>) {
                return Json();
            } else {
                return Json(result);
            }

            return out;
        }

        Json operator()(const Json& args) override {
            using ArgsTuple = FunctionTraits<F>::ArgsTuple;
            return invokeWithTuple(args, ArgsTuple{});
        }

        const Metadata& getMetadata() override {
            return mMetaData;
        }

        const string& getDescription() override {
            return mDescription;
        }

    private:
        Metadata mMetaData;
        string mDescription;
        F mFunction;
    };

    std::unordered_map<string, std::unique_ptr<IFunction>> mRequestableFunctions{};

public:
    static McpFunctionRegistry& Get() {
        static McpFunctionRegistry localINTERFACE{};
        return localINTERFACE;
    }

    Json getAllFunctions() {
        Json out = Json::array();
        for (const auto& function: mRequestableFunctions | std::views::values) {
            Json func;
            auto& meta = function->getMetadata();
            
            func["name"] = meta.name;
            func["description"] = function->getDescription();
            func["inputSchema"]["type"] = "object";

            for (const auto&[type, name]: meta.parameters) {
                func["inputSchema"]["properties"][name] = {
                    {"type", getJsonName(type)}
                };
                func["inputSchema"]["required"] += name;
            }
            out.push_back(func);
        }
        return out;
    }

    Json invokeFunction(const string& name, const Json& params) {
        const auto it = mRequestableFunctions.find(name);
        if (it == mRequestableFunctions.end()) throw std::runtime_error("Function not found");

        const std::unique_ptr<IFunction>& func = it->second;

        Json result = (*func)(params);

        Json out = {
            {"resultType", "complete"},
            {
                "content", {
                    result
                }
            }
        };

        return out;
    }

    template<typename F>
    bool registerFunction(F function, const std::string& description, const std::string& signature) {
        const auto metadata = ggm::Parsing::parseFunction(signature);

        mRequestableFunctions[metadata.name] = std::make_unique<FunctionWrapper<F>>(function, metadata, description);
        return false;
    }

#define MakeRequestableFunction(funcPtr, description, ...)\
    __VA_ARGS__;\
    inline static const bool MCP_UNIQUE_NAME(_mcp_registrated_) = \
        mcp::McpFunctionRegistry::Get().registerFunction(funcPtr, description, #__VA_ARGS__);\
        __VA_ARGS__

#define MakeRequestableFunction_class(funcPtr, description, ...)\
    inline static const bool MCP_UNIQUE_NAME(_mcp_registrated_) = \
        mcp::McpFunctionRegistry::Get().registerFunction(funcPtr, description, #__VA_ARGS__);\
        __VA_ARGS__

};
};

