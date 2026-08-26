#include <iostream>

#include "Network/Server.hpp"
#include "Mcp/McpFunctionRegistry.hpp"
#include "Mcp/McpResourceRegistry.hpp"
#include "Mcp/ReturnTypes.hpp"
#include "Utils/Parsing.hpp"

using Json = nlohmann::json;
using namespace mcp;

MakeRequestableFunction(testerFunc, "Returns the square root",
nlohmann::json testerFunc(int i)){
    return ReturnTypes::asText<>(i*i);
}

int main() {
    std::cout << "Starting...\n";
    McpResourceRegistry::Get().addResource(
        "test",
        "resource://test",
        McpResourceRegistry::MimeType::TEXT,
        [](const Json&) { return "Hello World";}
    );
    Server server{"hellow"};
    server.startAsync();

    while (server.isRunning())
    {
        Sleep(100);
    }
    
}