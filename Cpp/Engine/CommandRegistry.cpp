//
// Created by ertls on 05.03.2026.
//

#include <string>
#include <string_view>
#include <vector>
#include "CommandRegistry.hpp"

#include <array>
#include <functional>
#include <optional>
#include <variant>

namespace Engine {
    using std::string, std::string_view, std::array, std::vector, std::function, std::variant;

    enum class State {
        start,
        plainText,
        command,
        argument
    };

    enum class CommandState {
        operation,
        argument
    };


    struct Transition {
        variant<function<bool(State s)>, State> prevState;
        variant<function<bool(char c)>, char> event;
        State nextState;
        std::optional<function<void(vector<Token>& t, const string& s)>> actionFun;
    };

    static bool prevMatches(const variant<function<bool(State s)>, State>& v, const State s) {
        if (std::holds_alternative<State>(v)) {
            return std::get<State>(v) == s;
        }
        const auto pred = std::get<std::function<bool(State)>>(v);
        return pred ? pred(s) : false;
    }

    static bool charMatches(const variant<function<bool(char c)>, char>& v, const char s) {
        if (std::holds_alternative<char>(v)) {
            return std::get<char>(v) == s;
        }
        const auto pred = std::get<std::function<bool(char)>>(v);
        return pred ? pred(s) : false;
    }

    void procChar(char ch, vector<Token>& tokens, const string& str) {
        static auto state = State::start;
        static int begin = 0, end = 0;

        static auto newCommandToken = [](vector<Token>& t, const string& s){if (end != begin) t.emplace_back(TokenType::command, string_view(s.data()+begin, end - begin)); begin = ++end;};
        static auto newArgumentToken = [](vector<Token>& t, const string& s){if (end != begin) t.emplace_back(TokenType::argument, string_view(s.data()+begin, end - begin)); begin = ++end;};
        static auto newTextLiteralToken = [](vector<Token>& t, const string& s){if (end != begin) t.emplace_back(TokenType::textLiteral, string_view(s.data()+begin, end - begin)); begin = ++end;};

        static auto moveBeginAndEnd = [](vector<Token>& t, const string& s){begin++; end++;};
        static auto moveEnd = [](vector<Token>& t, const string& s){end++;};

        static auto anyState = [](State s){return true;};
        static auto anyChar = [](char c){return true;};

        static auto isValidForArgs = [](char c){return isdigit(c) || isalpha(c) || c == '-' || c == '.';};

        static auto resetState = [](vector<Token>& t, const string& s){begin = end = 0;};

        static Transition transitions[] = {
            {State::start, '/', State::command, moveBeginAndEnd},
            {State::start, [](char c){return c != '/';}, State::plainText, moveEnd},

            {State::plainText, anyChar, State::plainText, moveEnd},
            {State::plainText, '\n', State::plainText, newTextLiteralToken},

            {State::command, isalpha, State::command, moveEnd},
            {State::command, isspace, State::argument, newCommandToken},
            {State::command, '\n', State::command, newCommandToken},

            {State::argument, isValidForArgs, State::argument, moveEnd},
            {State::argument, isspace, State::argument, newArgumentToken},
            {State::argument, '\n', State::argument, newArgumentToken},

            {anyState, '\0', State::start, resetState}
        };

        for (auto &t : transitions) {
            if (prevMatches(t.prevState, state) && charMatches(t.event, ch)) {
                if (t.actionFun.has_value()) t.actionFun.value()(tokens, str);
                state = t.nextState;
                break;
            }
        }
    }

    std::pair<std::string, std::vector<std::string>> extractCommandAndArgs(const std::vector<Token>& tokens) {
        if (tokens.empty() || tokens[0].type != TokenType::command) {

            throw std::runtime_error("No command found");
        }

        auto command = std::string(tokens[0].value);
        std::vector<std::string> args;

        for (size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i].type == TokenType::argument) {
                args.emplace_back(tokens[i].value);
            }
        }

        return {command, args};
    }

    vector<Token> tokenize(const string& command) {
        vector<Token> tokens{};
        tokens.reserve(100);
        for (const auto c : command) {
            procChar(c, tokens, command);
        }
        procChar('\n', tokens, command);
        procChar('\0', tokens, command);

        return tokens;
    }

} // Engine