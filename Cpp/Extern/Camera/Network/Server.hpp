//
// Created by ertls on 27.07.2026.
//

#pragma once
#define CPPHTTPLIB_USE_WIN32
#include "httplib.hpp"
#include <string>
#include "json.hpp"

class Server {
    using Json = nlohmann::basic_json<>;
    using string = std::string;

public:
    explicit Server();

    static Json handleRequest(Json json);

    void start();

private:
    httplib::Server mServer;

    static Json getAllMethods();
};