//
// Created by ertls on 09.03.2026.
//

#ifndef MIXEDPROJECT_SQUARECOLLIDER_HPP
#define MIXEDPROJECT_SQUARECOLLIDER_HPP
#include "Utils/Math/Matrix.hpp"
#include "Utils/Math/Vector.hpp"

namespace Obj {
    struct Collider {
        struct ContactInfo {
            bool overlap = false;
            ggm::Vector3f normal{0,0,0};
            float penetration = 0.0f;
            ggm::Vector3f point{0,0,0};
        };

        ggm::Vector3f center{};
        ggm::Vector3f localCenter{};

        ggm::Vector3f axisX{};
        ggm::Vector3f axisY{};
        ggm::Vector3f axisZ{};

        ggm::Vector3f localAxisX{};
        ggm::Vector3f localAxisY{};
        ggm::Vector3f localAxisZ{};

        ggm::Vector3f halfSides{};
        ggm::Vector3f localHalfSides{};

        Collider(ggm::Vector3f center, float width, float height, float length, ggm::Vector3f rotation);
        ~Collider();

        ContactInfo isOverlap(Collider &other);

        void update(const ggm::Matrix4f &M);
    };
} // Obj

#endif //MIXEDPROJECT_SQUARECOLLIDER_HPP