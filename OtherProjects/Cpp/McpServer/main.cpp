#include <iostream>

#include "Network/Server.hpp"
#include "Mcp/McpFunctionRegistry.hpp"
#include "Mcp/ReturnTypes.hpp"
#include "Utils/Parsing.hpp"

MakeRequestableFunction(testerFunc, nlohmann::json testerFunc(int i)){
    return ReturnTypes::asText<>(i*i);
}

int main() {
    std::cout << "Starting...\n";
    Server server{"hellow"};
    server.run();
}