#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <iostream>
#include <string>
#include <thread>
#include <cstring>

#include "GuiInterface.h"
#include "SessionInfo.h"
#include "Camera/CameraCalculations.hpp"
#include "CameraGUI/CameraGUI.h"

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to start WinSock\n";
        return EXIT_FAILURE;
    }

    auto [width, height] = CalculateObjectPixelSize(0.1, 0.1, 1.0);

    std::cout << "Width: " << width << ", Height: " << height;

    Engine_loop(Camera_generateGUI);
    return EXIT_SUCCESS;

    WSACleanup();
}

