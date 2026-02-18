//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_VECTOR_H
#define MIXEDPROJECT_VECTOR_H

namespace Math {
    class Vector3f {
    public:
        Vector3f(){x = y = z = 0;}
        Vector3f(float x, float y, float z);
        Vector3f operator+(const Vector3f& v) const;
        Vector3f operator-(const Vector3f& v) const;
        Vector3f operator*(float factor) const;

        [[nodiscard]] Vector3f dot(const Vector3f &v) const;

        float x;
        float y;
        float z;
    };

    class Vector2f {
    public:
        Vector2f(float x, float y);
        float x;
        float y;
    };
}


#endif //MIXEDPROJECT_VECTOR_H