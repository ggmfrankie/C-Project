//
// Created by Stefan on 22.01.2026.
//

#include "Network.h"

#include <stdio.h>

#include "Utils/Logging/Logging.h"

#ifdef _WIN32
void initSockets() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) ERROR_("WSAStartup failed.\n");
}

SOCKET createServerSocket(const int port) {
    const SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) ERROR_("Socket was invalid");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) < 0) ERROR_("Binding failed");
    if (listen(serverSock, 1) < 0) ERROR_("Listening failed");

    printf("Waiting for client...\n");
    const SOCKET clientSock = accept(serverSock, nullptr, nullptr);
    closesocket(serverSock);
    printf("Client connected!\n");
    return clientSock;
}

SOCKET createClientSocket(const char* ip, const int port) {
    const SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) ERROR_("Socket creation failed");

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) ERROR_("Connection failed");

    printf("Connected to server!\n");
    return sock;
}

int sendData(SOCKET sock, const void* buffer, const int len) {
    return send(sock, buffer, len, 0);
}

int receiveData(SOCKET sock, void* buffer, const int len) {
    return recv(sock, buffer, len, 0);
}
#endif
