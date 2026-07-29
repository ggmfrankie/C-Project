//
// Created by ertls on 27.07.2026.
//

#include "Server.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>
#include <utility>

#include "../Dependencies/json/json.hpp"
#include "../Test/TicTacToe.hpp"
#include "../Utils/McpFunctionRegistry.hpp"


namespace Utils {
    class Json;
}

using httplib::Request;
using httplib::Response;
using std::string, std::optional;
using nlohmann::json;

Server::Server(string name, optional<string> authToken):
    mName(std::move(name)),
    mAuthBearerToken(std::move(authToken))
{
    if (!mAuthBearerToken.has_value()) {
        mAuthBearerToken = "mcp_test";
    }
}

optional<Server::Json> Server::handleRequest(Json json) {
    static bool initialized = false;
    const string method = json["method"];
    const int id = json["id"];

    std::cout << method << "\n";
    if (method == "initialize") return generateServerInformation(id);

    if (method == "notifications/initialize") {
        initialized = true;
        return std::nullopt;
    }
    if (!initialized) return generateError(id, -33, "Client is not initialized");

    if (method == "tools/list") return generateAllMethodsInformation(id);

    if (method == "tools/call") {

    }

    return std::nullopt;
}

Server::Json Server::generateRequest(int id, const string &method, std::initializer_list<Parameter> params) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"method", method}
    };
    for (auto&[type, name]: params) {
        out["params"] += {type, name};
    }
    return out;
}

Server::Json Server::generateResponse(int id, const Json& result) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"id", id}
    };
    out += result;
    return out;
}

Server::Json Server::generateError(int id, int code, const string &message, const std::optional<Json>& data) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"id", id}
    };
    out["error"] = {
        {"code", code},
        {"message", message}
    };
    if (data.has_value()) out["error"] += *data;

    return out;
}

Server::Json Server::generateNotification(const string &method, std::initializer_list<Parameter> params) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"method", method}
    };
    for (auto&[type, name]: params) {
        out["params"] += {type, name};
    }
    return out;
}

Server::Json Server::generateServerInformation(int id) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"result",
            {"protocolVersion", "2025-11-25"}
        }
    };
    out["result"]["capabilities"] = {
        {"logging", {}},
        {"prompts",
            {"listChanged", true}
        },
        {"resources",
            {"subscribe", false},
            {"listChanged", true}
        },
        {"tools",
            {"listChanged", true}
        },
        {"tasks",
            {"list", {}},
            {"cancel", {}},
            {"request",
                {"tools",
                    {"call", {}}
                }
            }
        }
    };
    out["serverInfo"] = {
        {"name", mName},
        {"title", mName},
        {"version", "1.0.0"},
        {"description", "server"}
    };

    return out;
}

void Server::start() {
    mServer.Post("/mcp",
        [this](const Request& req, Response& res) {
            if (!isAuthorized(req, res)) {
                return;
            }

            Json request;
            try {
                request = json::parse(req.body);
            } catch (const json::parse_error& e) {
                const Json error = generateError(-1, -32700, "Parse Error", e.what());
                res.status = 400;
                res.set_content(error.dump(), "application/json");
                return;
            }

            std::cout << "Request:\n" << request.dump(2) << '\n';

            if (const optional<Json> response = handleRequest(request);
                response.has_value()
            ){
                std::cout << "Response:\n" << response->dump(2) << '\n';

                res.set_content(response->dump(), "application/json");
            }
        }
    );

    mServer.listen("localhost", 8080);
}

void Server::setAuthToken(const string& token) {
    mAuthBearerToken = token;
}

void Server::clearAuthToken() {
    mAuthBearerToken = std::nullopt;
}

bool Server::isAuthorized(const Request& req, Response& res) const {
    if (!mAuthBearerToken.has_value()) {
        return true;
    }

    const string authHeader = req.get_header_value("Authorization");
    constexpr std::string_view bearerPrefix = "Bearer ";

    if (!authHeader.starts_with(bearerPrefix)) {
        const Json error = generateError(-1, -32001, "Unauthorized");

        res.status = 401;
        res.set_header("WWW-Authenticate", "Bearer realm=\"mcp\"");
        res.set_content(error.dump(), "application/json");
        return false;
    }

    if (const string token = authHeader.substr(bearerPrefix.size()); token != *mAuthBearerToken) {
        const Json error = generateError(-1, -32001, "Unauthorized");

        res.status = 401;
        res.set_header("WWW-Authenticate", "Bearer realm=\"mcp\", error=\"invalid_token\"");
        res.set_content(error.dump(), "application/json");
        return false;
    }

    return true;
}

Server::Json Server::generateAllMethodsInformation(int id) {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = id;


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
