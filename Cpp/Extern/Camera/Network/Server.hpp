//
// Created by ertls on 27.07.2026.
//

#pragma once
#define CPPHTTPLIB_USE_WIN32
#include "httplib.hpp"
#include "json.hpp"
#include "../Test/TicTacToe.hpp"

class Server {
    using Json = nlohmann::basic_json<>;
public:
    explicit Server();

    static Json handleRequest(Json json);

    void start();

private:
    httplib::Server mServer;
    TicTacToe mGame;

    static constexpr Json getAllMethods();

    template<typename F, typename... Args>
    Json invokeMethod(F&& method, Args&&... args) {
        using Result = std::invoke_result_t<F, Args...>;

        auto result = std::invoke(
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
};