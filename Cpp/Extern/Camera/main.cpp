#include <iostream>

#include "Network/Server.hpp"

int main(){
    std::cout << "Starting...\n";
    Server server{};
    std::cout << typeid(std::string).name() <<"\n";
    std::cout << typeid(bool).name() <<"\n";
    std::cout << typeid(int).name() <<"\n";
    //server.start();
}
