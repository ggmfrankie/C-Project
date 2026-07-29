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

    std::optional<Json> handleRequest(Json json);
    static Json generateRequest(int id, const string& method, std::initializer_list<Parameter> params = {});
    static Json generateResponse(int id, const Json& result);
    static Json generateError(int id, int code, const string& message, const std::optional<Json>& data = std::nullopt);
    static Json generateNotification(const string& method, std::initializer_list<Parameter> params = {});
    Json generateServerInformation(int id);

    void start();
    void setAuthToken(const string& token);
    void clearAuthToken();

private:
    httplib::Server mServer;
    string mName;
    std::optional<string> mAuthBearerToken;

    static Json generateAllMethodsInformation(int id);
    bool isAuthorized(const httplib::Request& req, httplib::Response& res) const;
};
