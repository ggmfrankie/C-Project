//
// Created by ertls on 27.07.2026.
//

#include "Server.hpp"

#include "json.hpp"
#include "../Test/TicTacToe.hpp"


using httplib::Request;
using httplib::Response;
using std::string;
using nlohmann::json;

Server::Server()
{
    TicTacToe::New();
}

Server::Json Server::handleRequest(Json json) {
    const string method = json["method"];

    std::cout << method << "\n";

    if (method == "tools/list") {
        return getAllMethods();
    }
    if (method == "tools/call") {
        string name = json["params"]["name"];
        if (name == "getBoard") {
            //return invokeMethod(&TicTacToe::getBoard, TicTacToe::Get());
        } else if (name == "makeMove") {
            int row = json["params"];
            //return invokeMethod(&TicTacToe::makeMove, TicTacToe::Get(), );
        }
    }

    return Json{"Uwu", 101};
}

void Server::start() {
    mServer.Post("/mcp",
        [](const Request& req, Response& res) {
            const Json request = json::parse(req.body);

            std::cout << "Request:\n" << request.dump(2) << '\n';

            const Json response = handleRequest(request);

            std::cout << "Response:\n" << response.dump(2) << '\n';

            res.set_content(response.dump(), "application/json");
        }
    );

    mServer.listen("localhost", 8080);
}

Server::Json Server::getAllMethods() {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = 1;
    out["result"]["tools"] = {
        {
            {"name", "getBoard"},
            {"description", "Returns current Board as String"}
        },
        {
            {"name", "makeMove"},
            {"description", "Makes a Move"},
            {"inputSchema",
                {
                    {"type", "object"},
                    {"properties",
                        {
                            {"row", {{"type", "integer"}, {"description", "Row of the Move"}}},
                            {"col", {{"type", "integer"}, {"description", "Col of the Move"}}}
                        }
                    },
                    {"required", {"row", "col"}}
                }
            }
        }
    };

    return out;
}

