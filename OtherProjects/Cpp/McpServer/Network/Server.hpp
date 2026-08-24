//
// Created by ertls on 27.07.2026.
//

#pragma once
#define CPPHTTPLIB_USE_WIN32
#include "httplib.hpp"
#include <string>
#include <optional>
#include "../Dependencies/json/json.hpp"
#include "../Utils/Parsing.hpp"

class Server {
    using Json = nlohmann::basic_json<>;
    using string = std::string;
    using Parameter = ggm::Parsing::Parameter;
public:
    explicit Server(string name, std::optional<string> authToken = std::nullopt);

    Json handleRequest(Json payload);
    static Json generateError(const Json &id, int code, const string& message, const std::optional<Json>& data = std::nullopt);
    Json generateServerInformation(const Json &id);

    static Json toolsList(const Json& id);
    static Json toolsCall(const Json& id, const Json &params);
    static Json resourcesList(const Json& id);

    void run();
    void setAuthToken(const string& token);
    void clearAuthToken();

private:
    httplib::Server mServer;
    string mName;
    string mIp;
    int mPort;
    std::optional<string> mAuthBearerToken;

    bool isAuthorized(const httplib::Request& req, httplib::Response& res) const;
};
