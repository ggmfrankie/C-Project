//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_MATRIX_H
#define MIXEDPROJECT_MATRIX_H
#include <array>
#include <cmath>
#include <numbers>

#include "Quaternion.h"
#include "Vector.h"

namespace Math {
    class Matrix4f {
        public:
        constexpr Matrix4f() = default;

        constexpr explicit Matrix4f(const std::array<float, 16> &a): m(a){}

        constexpr explicit Matrix4f(const std::array<float, 16> &&a): m(a){}
        static constexpr auto toRad(const float degree){return degree * (static_cast<float>(std::numbers::pi) / 180);};

        static constexpr Matrix4f Identity() noexcept {
            Matrix4f I{};
            I.m = {
                    1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1
            };
            return I;
        }

        static constexpr Matrix4f Scale(const float scale) noexcept {
            Matrix4f M{};
            M.m = {
                    scale,0,0,0,
                    0,scale,0,0,
                    0,0,scale,0,
                    0,0,0,1
            };
            return M;
        }

        static constexpr Matrix4f Translation(const float x, const float y, const float z) noexcept {
            Matrix4f M{};
            M.m = {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                x,y,z,1
            };
            return M;
        }


        static constexpr Matrix4f Translation(const Vector3f pos) noexcept {
            Matrix4f M{};
            M.m = {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                pos.x,pos.y,pos.z,1
        };
            return M;
        }

        static constexpr Matrix4f RotationX(const float degrees) noexcept {
            using std::cos, std::sin;
            const auto rad = toRad(degrees);
            Matrix4f I{};
            I.m = {
                1,0,0,0,
                0, cos(rad), -sin(rad), 0,
                0, sin(rad),  cos(rad), 0,
                0,0,0,1
            };
            return I;
        }

        static constexpr Matrix4f RotationY(const float degrees) noexcept {
            using std::cos, std::sin;
            const auto rad = toRad(degrees);
            Matrix4f I{};
            I.m = {
                cos(rad), 0, sin(rad), 0,
                0, 1, 0, 0,
                -sin(rad), 0, cos(rad), 0,
                0,0,0,1
            };
            return I;
        }

        static constexpr Matrix4f RotationZ(const float degrees) noexcept {
            using std::cos, std::sin;
            const float rad = toRad(degrees);
            Matrix4f I{};
            I.m = {
                cos(rad), -sin(rad), 0, 0,
                sin(rad),  cos(rad), 0, 0,
                0,0,1,0,
                0,0,0,1
            };
            return I;
        }

        static constexpr Matrix4f Perspective(const float fovD, const float aspect, const float zNear, const float zFar) {
            using std::tan;
            const float fov = toRad(fovD);
            const float f = 1.0f / tan(fov / 2.0f);

            Matrix4f I{};
            I.m = {
                f / aspect, 0, 0, 0,
                0, f, 0, 0,
                0, 0, (zFar + zNear) / (zNear - zFar), -1,
                0, 0, (2 * zFar * zNear) / (zNear - zFar), 0
            };
            return I;
        }

        [[nodiscard]] std::array<float, 16> getData() const {
            return m;
        }

        [[nodiscard]] const float *getDataPtr() const noexcept {
            return m.data();
        }

        friend constexpr Matrix4f operator+(const Matrix4f& a, const Matrix4f& b) noexcept {
            Matrix4f r{};
            for (std::size_t i = 0; i < 16; ++i)
                r.m[i] = a.m[i] + b.m[i];
            return r;
        }

        friend constexpr Matrix4f operator-(const Matrix4f& a, const Matrix4f& b) noexcept {
            Matrix4f r{};
            for (std::size_t i = 0; i < 16; ++i)
                r.m[i] = a.m[i] - b.m[i];
            return r;
        }

        friend constexpr Matrix4f operator*(const Matrix4f& a, const float s) noexcept {
            Matrix4f r{};
            for (std::size_t i = 0; i < 16; ++i)
                r.m[i] = a.m[i] * s;
            return r;
        }

        friend constexpr Matrix4f operator*(const float s, const Matrix4f& a) noexcept {
            return a * s;
        }


        friend constexpr Matrix4f operator*(const Matrix4f& A, const Matrix4f& B) noexcept {
            Matrix4f R{};

            for (std::size_t col = 0; col < 4; ++col) {
                for (std::size_t row = 0; row < 4; ++row) {
                    float sum = 0.0f;
                    #pragma omp simd reduction(+:sum)
                    for (std::size_t k = 0; k < 4; ++k) {
                        sum += A.m[k * 4 + row] * B.m[col * 4 + k];
                    }
                    R.m[col * 4 + row] = sum;
                }
            }
            return R;
        }

        constexpr float& at(const std::size_t col, const std::size_t row) noexcept {
            return m[col * 4 + row];
        }

        [[nodiscard]] constexpr float at(const std::size_t col, const std::size_t row) const noexcept {
            return m[col * 4 + row];
        }



        ~Matrix4f() = default;

        private:
        std::array<float, 16> m;
    };
} // Math

#endif //MIXEDPROJECT_MATRIX_H