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
#include "../Mcp/McpResourceRegistry.hpp"


namespace Utils {
    class Json;
}

using httplib::Request;
using httplib::Response;
using std::string, std::optional;
using nlohmann::json;

namespace mcp {
Server::Server(string name, optional<string> authToken):
    mServer(),
    mName(std::move(name)),
    mIp("localhost"),
    mPort(8080),
    mSseQueue()
{}

Server::~Server() {
    stop();
}

std::optional<Server::Json> Server::handleRequest(Json payload) {
    const string method = payload["method"];

    if (!payload.contains("id")) {
        if (method == "notifications/initialized") return std::nullopt;
        return generateError("-2", -300, "Notification not implemented");
    }

    const Json& id = payload["id"];
    const Json& params = payload["params"];

    std::cout << method << "\n";
    if (method == "initialize") return generateServerInformation(id);

    if (method == "server/discover") return generateServerInformation(id);

    if (method == "tools/list") return toolsList(id);

    if (method == "tools/call") return toolsCall(id, params);

    if (method == "resources/list") return resourcesList(id);

    if (method == "resources/read") return resourcesRead(id, params);

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

Server::Json Server::getMeta() {
    return Json {
        "_meta", {
            {"io.modelcontextprotocoll/protocolVersion", PROTOCOL_VERSION},
            {"io.modecontextprotocol/serverInfo", {
                {"name", mName},
                {"version", "1.0.0"}
            }}
        }
    };
}

Server::Json Server::generateServerInformation(const Json &id) {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = id;
    out["result"] = {
        {"protocolVersion", PROTOCOL_VERSION},
        {"capabilities", {
            {"tools", Json::object()},
            {"resources", {
                {"listChanged", true}
            }},
            {"logging", Json::object()}
        }},
        {"serverInfo", {
            {"name", mName},
            {"version", "1.0.0"}
        }}
    };

    return out;
}

void Server::run() {
    mRunning.store(true, std::memory_order_release);

    mServer.Post("/mcp",
        [this](const Request& req, Response& res) {
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

            std::optional<Json> response;
            try {
                response = handleRequest(request);
            } catch (const std::exception& e){
                std::cerr << "Unhandled exception: " << e.what() << '\n';
                const Json err = generateError(request.value("id", nullptr), -32603, "Internal error", e.what());
                res.status = 200;
                res.set_content(err.dump(), "application/json");
                return;
            }

            if (!response.has_value()) return;
            std::cout << "Response:\n" << response->dump(2) << '\n';
            res.set_content(response->dump(), "application/json");
        }
    );

    mServer.Get("/mcp", [this](const Request&, Response& res) {
        res.set_chunked_content_provider("text/event-stream",
            [this](size_t, httplib::DataSink& sink) {
                std::unique_lock lock(mSseMutex);
                mSseCv.wait(lock, [this] { return !mSseQueue.empty() || !mRunning; });

                if (!mRunning) return false;

                Json notification = mSseQueue.front();
                mSseQueue.pop();
                lock.unlock();

                const string data = "data: " + notification.dump() + "\n\n";
                return sink.write(data.c_str(), data.size());
            }
        );
    });

    std::cout << "Listening on: " << mIp << ":" << mPort << "\n";
    mServer.listen(mIp, mPort);
    mRunning.store(false, std::memory_order_release);
}

void Server::startAsync() {
    if (mThread.joinable()) return;
    mThread = std::jthread([this](std::stop_token) {
        mRunning.store(true, std::memory_order_release);
        this->run();
        mRunning.store(false, std::memory_order_release);
    });
}

bool Server::isRunning() const {
    return mRunning.load(std::memory_order_acquire);
}

void Server::stop() {
    mRunning.store(false, std::memory_order_release);
    mSseCv.notify_all();
    mServer.stop();
    if (mThread.joinable()) {
        mThread.request_stop();
        mThread.join();
    }
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
    
    out["result"] = {
        {"resultType", "complete"},
        {"resources", McpResourceRegistry::Get().listResources()},
        {"ttlMs", 300000},
        {"cacheScope", "public"}
    };

    return out;
}

Server::Json Server::resourcesRead(const Json &id, const Json &params) {
    Json out;
    out["jsonrpc"] = "2.0";
    out["id"] = id;
    
    out["result"] = {
        {"resultType", "complete"}
    };
    return out;
}

void Server::notify(const Json &notification) {
    {
        std::lock_guard lock(mSseMutex);
        mSseQueue.push(notification);
    }
    mSseCv.notify_all();
}
}