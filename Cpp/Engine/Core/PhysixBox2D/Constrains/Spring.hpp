//
// Created by Stefan on 27.03.2026.
//

#pragma once


namespace PhysixBox {
    struct  PointMass;

    struct Spring {
        PointMass& first;
        PointMass& second;

        Spring(PointMass& first, PointMass& second, float strength, float length, float damping);

        float strength = 0;
        float length = 0;
        float damping = 0;
    };
} // PhysixBox