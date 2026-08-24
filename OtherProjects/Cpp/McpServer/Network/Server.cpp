//
// Created by ertls on 27.07.2026.
//

#include "Server.hpp"

#include <cstdlib>
#include <iostream>
#include <string_view>
#include <utility>
#include <cassert>

#include "../Dependencies/json/json.hpp"
#include "../Test/TicTacToe.hpp"
#include "../Mcp/McpFunctionRegistry.hpp"


namespace Utils {
    class Json;
}

using httplib::Request;
using httplib::Response;
using std::string, std::optional;
using nlohmann::json;

Server::Server(string name, optional<string> authToken):
    mServer(),
    mName(std::move(name)),
    mIp("localhost"),
    mPort(8080),
    mAuthBearerToken(std::move(authToken))
{
    if (!mAuthBearerToken.has_value()) {
        mAuthBearerToken = "mcp_test";
    }
}

Server::Json Server::handleRequest(Json payload) {
    static bool initialized = true;
    const string method = payload["method"];
    const Json id = payload["id"];

    std::cout << method << "\n";
    if (method == "initialize") return generateServerInformation(id);

    if (method == "server/discover") return generateServerInformation(id);

    // if (method == "notifications/initialize") {
    //     initialized = true;
    //     return std::nullopt;
    // }

    if (!initialized) return generateError(id, -33, "Client is not initialized");

    if (method == "tools/list") return toolsList(id);

    if (method == "tools/call") return toolsCall(id, payload["params"]);

    return generateError(id, -200, "Method not implemented");
}

Server::Json Server::generateError(const Json &id, int code, const string &message, const std::optional<Json>& data) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"id", id}
    };
    out["error"] = {
        {"code", code},
        {"message", message}
    };
    if (data.has_value()) out["error"] = *data;

    return out;
}

Server::Json Server::generateServerInformation(const Json &id) {
    Json out = {
        {"jsonrpc", "2.0"},
        {"id", id},
        {"result", {
            {"resultType", "complete"},
            {"supportedVersions", {"2026-07-28"}},
        }}
    };
    out["result"]["capabilities"] = {
        {"tools", {
            {"listChanged", false}
        }}
    };
    out["result"]["_meta"]["io.modelcontextprotocol/serverInfo"] = {
        {"name", mName},
        {"version", "1.0.0"},
    };

    return out;
}

void Server::run() {
    mServer.Post("/mcp",
        [this](const Request& req, Response& res) {
            if (!isAuthorized(req, res)) {
                return;
            }

            Json request;
            try {
                request = json::parse(req.body);
            } catch (const json::parse_error& e) {
                const Json error = generateError("Error", -32700, "Parse Error", e.what());
                res.status = 400;
                res.set_content(error.dump(), "application/json");
                return;
            } catch (const std::exception& e) {
                std::cerr << "Unhandled exception: " << e.what() << '\n';
                const Json err = generateError(request.value("id", nullptr), -32603, "Internal error", e.what());
                res.status = 200;
                res.set_content(err.dump(), "application/json");
                return;
            }

            std::cout << "Request:\n" << request.dump(2) << '\n';

            Json response;
            try {
                response = handleRequest(request);
            } catch (const std::exception& e){
                std::cerr << "Unhandled exception: " << e.what() << '\n';
                const Json err = generateError(request.value("id", nullptr), -32603, "Internal error", e.what());
                res.status = 200;
                res.set_content(err.dump(), "application/json");
                return;
            }
            
            std::cout << "Response:\n" << response.dump(2) << '\n';

            res.set_content(response.dump(), "application/json");
        }
    );
    std::cout << "Listening on: " << mIp << ":" << mPort << "\n";
    mServer.listen(mIp, mPort);
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
        const Json error = generateError("Error", -32001, "Unauthorized");

        res.status = 401;
        res.set_header("WWW-Authenticate", "Bearer realm=\"mcp\"");
        res.set_content(error.dump(), "application/json");
        return false;
    }

    if (const string token = authHeader.substr(bearerPrefix.size()); token != *mAuthBearerToken) {
        const Json error = generateError("Error", -32001, "Unauthorized");

        res.status = 401;
        res.set_header("WWW-Authenticate", R"(Bearer realm="mcp", error="invalid_token")");
        res.set_content(error.dump(), "application/json");
        return false;
    }

    return true;
}

Server::Json Server::toolsList(const Json& id) {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = id;

    out["result"] = {
        {"resultType", "complete"},
        {"tools", McpFunctionRegistry::Get().getAllFunctions()},
        {"ttlMs", 300000},
        {"cacheScope", "public"}
    };

    return out;
}

Server::Json Server::toolsCall(const Json &id, const Json &params) {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = id;

    const string funcName = params["name"];
    const Json& args = params["arguments"];

    out["result"] = McpFunctionRegistry::Get().invokeFunction(funcName, args);

    return out;
}

Server::Json Server::resourcesList(const Json &id) {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = id;
    assert(false);
    //TODO
    return out;
}
