//
// Created by Stefan on 27.03.2026.
//

#pragma once
#include "../EngineDefines.hpp"
#include "PointMass.hpp"
#include "Utils/DataStorage/ArrayList.hpp"
#include <vector>

#include "Constrains/Spring.hpp"

namespace PhysixBox {
    class SoftBody2D {
        ggm::ArrayList<PointMass, 16> mPoints{};
        ggm::ArrayList<Spring, 16> mSprings{};

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

        ggm::ArrayList<PointMass, 16>& getPoints();
        [[nodiscard]] const ggm::ArrayList<PointMass, 16>& getPoints() const;
    };
}