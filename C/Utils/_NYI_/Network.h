//
// Created by Stefan on 22.01.2026.
//

#pragma once
#include "GuiDefines.h"
#ifdef WIN32
#include <winsock.h>
#include <psdk_inc/_socket_types.h>

void initSockets();
SOCKET createClientSocket(const char* ip, int port);
SOCKET createServerSocket(int port);

int sendData(SOCKET sock, const void* buffer, int len);
int receiveData(SOCKET sock, void* buffer, int len);
#endif