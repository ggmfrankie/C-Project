#include <iostream>

#include "Network/Server.hpp"

int main(){
    std::cout << "Hello World\n";
    Server server{};
    server.start();
}
