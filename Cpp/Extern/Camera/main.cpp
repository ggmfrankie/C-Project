#include <iostream>

#include "Network/Server.hpp"
#include "Utils/McpInterface.hpp"
#include "Utils/Parsing.hpp"

MakeRequestableFunction(int main())
{
    std::cout << "Starting...\n";
    Server server{};
    auto c = ggm::Parsing::extractParameters("hashas myFunction::ofdeez(int a, float b, const std::string& c)");
    std::cout << c.name << '\n';
    std::cout << c.returnType << '\n';
    for (const auto& parameter : c.parameters) {
        std::cout << parameter.name << ": " << parameter.type << "\n";
    }
    //server.start();
}
