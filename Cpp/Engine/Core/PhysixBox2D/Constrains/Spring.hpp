//
// Created by Stefan on 27.03.2026.
//

#pragma once


namespace PhysixBox {
    struct  PointMass;

    struct Spring {
        PointMass& first;
        PointMass& second;

        Spring(PointMass& first, PointMass& second, float strength, float length);
        void update();

    private:
        float mStrength = 0;
        float mLength = 0;
    };
} // PhysixBox