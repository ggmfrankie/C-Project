//
// Created by ertls on 27.07.2026.
//

#pragma once
#define CPPHTTPLIB_USE_WIN32
#include "httplib.hpp"
#include <string>
#include <optional>
#include <queue>
#include <deque>
#include <thread>
#include "../Dependencies/json/json.hpp"
#include "../Utils/Parsing.hpp"

class Server {
    using Json = nlohmann::json;
    using string = std::string;
    using Parameter = ggm::Parsing::Parameter;

    static Json toolsList(const Json& id);
    static Json toolsCall(const Json& id, const Json &params);

    static Json resourcesList(const Json& id);
    static Json resourcesRead(const Json& id, const Json& params);

    Json generateServerInformation(const Json &id);
    std::optional<Json> handleRequest(Json payload);

    static Json generateError(const Json &id, int code, const string& message, const std::optional<Json>& data = std::nullopt);
    Json getMeta();

public:
    explicit Server(string name, std::optional<string> authToken = std::nullopt);
    ~Server();

    void notify(const Json& notification);

    void run();
    void startAsync();
    void stop();

    [[nodiscard]] bool isRunning() const;

private:
    const std::string PROTOCOL_VERSION = "2026-07-28";

    httplib::Server mServer;
    string mName;
    string mIp;
    int mPort;

    std::jthread mThread;
    std::atomic<bool> mRunning{false};

    std::mutex mSseMutex;
    std::condition_variable mSseCv;
    std::queue<Json> mSseQueue;
};
