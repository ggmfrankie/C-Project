//
// Created by Stefan on 22.01.2026.
//

#include "Network.h"

#include <stdio.h>


void initSockets() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("WSAStartup failed.\n");
        exit(1);
    }
#endif
}

SOCKET createServerSocket(const int port) {
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET) { perror("socket"); exit(1); }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }

    if (listen(serverSock, 1) < 0) { perror("listen"); exit(1); }

    printf("Waiting for client...\n");
    SOCKET clientSock = accept(serverSock, NULL, NULL);
    if (clientSock < 0) { perror("accept"); exit(1); }
    closesocket(serverSock);
    printf("Client connected!\n");
    return clientSock;
}

SOCKET createClientSocket(const char* ip, const int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) { perror("socket"); exit(1); }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect"); exit(1);
    }

    printf("Connected to server!\n");
    return sock;
}

int sendData(SOCKET sock, const void* buffer, const int len) {
    return send(sock, buffer, len, 0);
}

int receiveData(SOCKET sock, void* buffer, const int len) {
    return recv(sock, buffer, len, 0);
}
