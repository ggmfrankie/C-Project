//
// Created by ertls on 05.03.2026.
//

#ifndef MIXEDPROJECT_COMMANDPARSER_HPP
#define MIXEDPROJECT_COMMANDPARSER_HPP
#include <memory>
#include <variant>

#include "Task.hpp"
#include "Utils/DataStorage/InlineVector.hpp"

namespace Engine {
    enum class TokenType {
        command,
        argument,
        textLiteral
    };

    struct Token {
        TokenType type;
        std::string_view value;

        Token(const TokenType t, const std::string_view value) :type(t), value(value) {}
    };

    std::vector<Token> tokenize(const std::string& command);
    std::pair<std::string, std::vector<std::string>> extractCommandAndArgs(const std::vector<Token>& tokens);

    template<typename T>
        struct Arg {using type = T;};

    template<typename T>
            static T convert(const std::string& s) {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(s);
        }
        else if constexpr (std::is_same_v<T, float>) {
            return std::stof(s);
        }
        else if constexpr (std::is_same_v<T, double>) {
            return std::stod(s);
        }
        else if constexpr (std::is_same_v<T, bool>) {
            return s == "1" || s == "true" || s == "yes";
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            return s;
        }
        else {
            static_assert(!sizeof(T), "No converter for this type");
            return T{};
        }
    }
    class CommandRegistry {
        struct ICommand {
            virtual ~ICommand() = default;
            virtual void run(const std::vector<std::string>& rawArgs) = 0;
        };

        template<typename Func, typename... Args>
        class Command final : public ICommand {

            template<std::size_t... I>
                void invokeImpl(const std::vector<std::string>& rawArgs,
                                std::index_sequence<I...>)
            {
                func(convert<typename Args::type>(rawArgs[I])...);
            }

            Func func;
        public:
            explicit Command(Func f) : func(f) {}

            void run(const std::vector<std::string> &rawArgs) override {
                if (rawArgs.size() != sizeof...(Args)) throw std::runtime_error("Invalid argument count");
                invokeImpl(rawArgs, std::index_sequence_for<Args...>{});
            }
        };

        std::unordered_map<std::string, std::unique_ptr<ICommand>> commands;

    public:
        template<typename... Args, typename Func>
        void registerCommand(const std::string& name, Func func) {
            using Cmd = Command<Func, Args...>;
            commands[name] = std::make_unique<Cmd>(func);
        }

        void run(const std::string& input) {
            const auto tokens = tokenize(input);

            auto [command, rawArgs] = extractCommandAndArgs(tokens);

            const auto it = commands.find(command);
            if (it == commands.end())
                throw std::runtime_error("Unknown command: " + command);

            it->second->run(rawArgs);
        }
    };
} // Engine

#endif //MIXEDPROJECT_COMMANDPARSER_HPP