//
// Created by ertls on 30.07.2026.
//

#pragma once
#include <string>
#include <winsock2.h>

#include "SessionInfo.h"
#include "MessageRouter.h"
#include "utils/Logger.h"
#include "utils/Buffer.h"

class Camera {
    using string = std::string;
    using ServiceCodes = eipScanner::cip::ServiceCodes;
    using EPath = eipScanner::cip::EPath;
    using GeneralStatusCodes = eipScanner::cip::GeneralStatusCodes;

    static constexpr uint16_t PORT  = 0xAF12;

    template<typename T>
    static T get(const std::vector<uint8_t>& d, const int offset) {
        if (offset + 4 > d.size()) return T{};
        T out;
        std::memcpy(&out, d.data() + offset, sizeof(T));
        return out;
    }

    const string mIP;
    std::shared_ptr<eipScanner::SessionInfo>mSessionInfo;
    eipScanner::MessageRouter mMessageRouter;
public:
    explicit Camera(const char* ip):
        mIP(string(ip)),
        mSessionInfo(new eipScanner::SessionInfo(ip, PORT, std::chrono::seconds(5))),
        mMessageRouter(true)
    {}

    struct ThermalData {
        static constexpr int INSTANCE = 0x64;
        static constexpr int ATTRIBUTE = 3;

        static ThermalData extract(const std::vector<uint8_t>& d) {
            ThermalData t;
            t.internalTemp = get<float>(d, 32);
            t.spot1        = get<float>(d, 36);
            t.box1Min      = get<float>(d, 40);
            t.box1Max      = get<float>(d, 44);
            t.box1Avg      = get<float>(d, 48);
            t.spot2        = get<float>(d, 56);
            t.spot3        = get<float>(d, 76);
            return t;
        }

        float internalTemp = 0.f;
        float spot1        = 0.f;
        float box1Min      = 0.f;
        float box1Max      = 0.f;
        float box1Avg      = 0.f;
        float spot2        = 0.f;
        float spot3        = 0.f;
    };

    template<typename T>
    T pollData() {
        const auto response = mMessageRouter.sendRequest(
            mSessionInfo,
            ServiceCodes::GET_ATTRIBUTE_SINGLE,
            EPath(0x04, T::INSTANCE, T::ATTRIBUTE)
        );

        if (response.getGeneralStatusCode() == GeneralStatusCodes::SUCCESS) {
            const std::vector<uint8_t>& data = response.getData();
            return T::extract(data);
        }
        std::ostringstream msg;
        msg << "[EIP] Error: 0x" << std::hex << response.getGeneralStatusCode() << '\n';
        throw std::runtime_error(msg.str());

    }
};
