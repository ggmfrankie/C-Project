#if defined(_WIN32) || defined(WIN32) || defined(_WIN64)
#include <winsock2.h>
#define OS_Windows (1)
#endif

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <cstring>

#include "SessionInfo.h"
#include "MessageRouter.h"
#include "Camera/Camera.hpp"
#include "utils/Logger.h"
#include "utils/Buffer.h"

using namespace eipScanner::cip;
using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

static constexpr const char* CAMERA_IP = "169.254.4.38";
static constexpr uint16_t EIP_PORT  = 0xAF12;  // 44818

// Byte offsets inside input assembly 0x64, attribute 3
static constexpr int OFF_INTERNAL  = 32;
static constexpr int OFF_SPOT1     = 36;
static constexpr int OFF_BOX1_MIN  = 40;
static constexpr int OFF_BOX1_MAX  = 44;
static constexpr int OFF_BOX1_AVG  = 48;
static constexpr int OFF_SPOT2     = 56;
static constexpr int OFF_SPOT3     = 76;

static constexpr int INPUT_OBJECT = 0x64;



int main() {
#if OS_Windows
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to start WinSock\n";
        return EXIT_FAILURE;
    }
#endif

    const std::string rtspSource = std::string("rtsp://") + CAMERA_IP + "/avc";
    Camera camera {CAMERA_IP};

    cv::VideoCapture cap(rtspSource, cv::CAP_FFMPEG);
    if (!cap.isOpened()) {
        std::cerr << "Could not open RTSP stream: " << rtspSource << '\n';
        return 1;
    }


    cv::namedWindow("InfraCamera", cv::WINDOW_NORMAL);

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Stream ended or frame read failed.\n";
            break;
        }

        auto [internalTemp, spot1, box1Min, box1Max, box1Avg, spot2, spot3] = camera.pollData<Camera::ThermalData>();
        auto put = [&](const std::string &label, const float val, int y) {
            char buf[64];
            std::snprintf(buf, sizeof(buf), "%s: %.1f C", label.c_str(), val);
            cv::putText(frame, buf, {10, y}, cv::FONT_HERSHEY_SIMPLEX,
                        0.6, {0, 255, 0}, 2);
        };

        put("Internal", internalTemp, 30);
        put("Spot1", spot1, 60);
        put("Box1 Min", box1Min, 90);
        put("Box1 Max", box1Max, 120);
        put("Box1 Avg", box1Avg, 150);
        put("Spot2", spot2, 180);
        put("Spot3", spot3, 210);

        cv::imshow("InfraCamera", frame);
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        if (const int key = cv::waitKey(1);
            key == 27 || key == 'q' || key == 'Q'
        )
            break;
    }

#if OS_Windows
    WSACleanup();
#endif

    return EXIT_SUCCESS;
}

