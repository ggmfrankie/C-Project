//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "PointMass.hpp"
#include "Utils/DataStorage/InlineVector.hpp"
#include <vector>

namespace PhysixBox {
    class SoftBody2D {
        ggm::InlineVector<PointMass, 16> mPoints{};
        ggm::InlineVector<Spring, 16> mSprings{};

    public:
        SoftBody2D(const std::vector<PointMass>& points, const std::vector<Spring>& springs);
        explicit SoftBody2D(const std::vector<PointMass>& points);
        SoftBody2D() = default;

        template<typename... Args>
        void addPoint(Args&&... args) {
            mPoints.push(std::forward<Args>(args)...);
        }

        template<typename... Args>
        void addSpring(Args&&... args) {
            mSprings.push(std::forward<Args>(args)...);
        }

        void update(float dt);
        PointMass& getPointMass(ggm::u64 index);

        std::vector<ggm::Vector2f> getPositions();
    };
} // PhysicsBox