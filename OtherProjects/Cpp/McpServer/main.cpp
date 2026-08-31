#include <iostream>

#include "Network/Server.hpp"
#include "Mcp/McpFunctionRegistry.hpp"
#include "Mcp/McpResourceRegistry.hpp"
#include "Mcp/ReturnTypes.hpp"
#include "Utils/Parsing.hpp"

#include "Test/TicTacToe.hpp"

using Json = nlohmann::json;
using namespace mcp;

MakeRequestableFunction(makeMove, "Makes a move and places the correct symbol at the requested spot. Returns the board state after the move or tells if move was invalid. If you won it also tells you",
nlohmann::json makeMove(int row, int col)){
    bool success = TicTacToe::Get().makeMove(row, col);
    TicTacToe::Get().printBoard();
    return ReturnTypes::asText(success ? (TicTacToe::Get().isWinning() ? "You won" : TicTacToe::Get().getBoard()) : "move invalid");
}

int main() {
    std::cout << "Starting..." << std::endl;
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