//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_MATRIX_H
#define MIXEDPROJECT_MATRIX_H
#include <array>
#include <cmath>
#include <numbers>

#include "ggmdef.hpp"
#include "Quaternion.hpp"
#include "Vector.hpp"

namespace ggm {
    class Matrix3f {
    public:

        constexpr Matrix3f() = default;

        constexpr explicit Matrix3f(const std::array<float, 9> &a): m(a){}

        constexpr explicit Matrix3f(const std::array<float, 9> &&a): m(a){}
    private:


        std::array<float, 9> m{};
    public:
        static constexpr int idx(int row, int col) { return col * 3 + row; }

        constexpr float  operator()(int row, int col) const { return m[idx(row, col)]; }
        constexpr float& operator()(int row, int col)       { return m[idx(row, col)]; }

        [[nodiscard]] constexpr Matrix3f inverse() const {

            const auto& a = m;

            float a00 = a[0], a01 = a[3], a02 = a[6];
            float a10 = a[1], a11 = a[4], a12 = a[7];
            float a20 = a[2], a21 = a[5], a22 = a[8];

            float det =
                a00 * (a11 * a22 - a12 * a21) -
                a01 * (a10 * a22 - a12 * a20) +
                a02 * (a10 * a21 - a11 * a20);

            if (std::fabs(det) < 1e-8f)
                return Matrix3f({1,0,0, 0,1,0, 0,0,1});

            float invDet = 1.0f / det;

            return Matrix3f({

                ( a11 * a22 - a12 * a21) * invDet,
                (-a10 * a22 + a12 * a20) * invDet,
                ( a10 * a21 - a11 * a20) * invDet,

                (-a01 * a22 + a02 * a21) * invDet,
                ( a00 * a22 - a02 * a20) * invDet,
                (-a00 * a21 + a01 * a20) * invDet,

                ( a01 * a12 - a02 * a11) * invDet,
                (-a00 * a12 + a02 * a10) * invDet,
                ( a00 * a11 - a01 * a10) * invDet
            });
        }

        [[nodiscard]] constexpr Matrix3f transpose() const {
            return Matrix3f({
                m[0], m[3], m[6],
                m[1], m[4], m[7],
                m[2], m[5], m[8]
            });
        }

        constexpr Matrix3f operator*(const Matrix3f& b) const {
            const Matrix3f& a = *this;
            return Matrix3f({
                a(0,0)*b(0,0) + a(0,1)*b(1,0) + a(0,2)*b(2,0),
                a(1,0)*b(0,0) + a(1,1)*b(1,0) + a(1,2)*b(2,0),
                a(2,0)*b(0,0) + a(2,1)*b(1,0) + a(2,2)*b(2,0),

                a(0,0)*b(0,1) + a(0,1)*b(1,1) + a(0,2)*b(2,1),
                a(1,0)*b(0,1) + a(1,1)*b(1,1) + a(1,2)*b(2,1),
                a(2,0)*b(0,1) + a(2,1)*b(1,1) + a(2,2)*b(2,1),

                a(0,0)*b(0,2) + a(0,1)*b(1,2) + a(0,2)*b(2,2),
                a(1,0)*b(0,2) + a(1,1)*b(1,2) + a(1,2)*b(2,2),
                a(2,0)*b(0,2) + a(2,1)*b(1,2) + a(2,2)*b(2,2)
            });
        }

        constexpr Matrix3f operator*(const float s) const {
            return Matrix3f({
                m[0]*s, m[1]*s, m[2]*s,
                m[3]*s, m[4]*s, m[5]*s,
                m[6]*s, m[7]*s, m[8]*s
            });
        }

        constexpr Matrix3f operator-(const Matrix3f& b) const {
            return Matrix3f({
                m[0]-b.m[0], m[1]-b.m[1], m[2]-b.m[2],
                m[3]-b.m[3], m[4]-b.m[4], m[5]-b.m[5],
                m[6]-b.m[6], m[7]-b.m[7], m[8]-b.m[8]
            });
        }

        constexpr Matrix3f operator+(const Matrix3f& b) const {
            return Matrix3f({
                m[0]+b.m[0], m[1]+b.m[1], m[2]+b.m[2],
                m[3]+b.m[3], m[4]+b.m[4], m[5]+b.m[5],
                m[6]+b.m[6], m[7]+b.m[7], m[8]+b.m[8]
            });
        }

        constexpr Vector3f operator*(const Vector3f& v) const {
            return {
                m[idx(0,0)] * v.x + m[idx(0,1)] * v.y + m[idx(0,2)] * v.z,
                m[idx(1,0)] * v.x + m[idx(1,1)] * v.y + m[idx(1,2)] * v.z,
                m[idx(2,0)] * v.x + m[idx(2,1)] * v.y + m[idx(2,2)] * v.z
            };
        }

        static constexpr Matrix3f FromColumns(const Vector3f& cx, const Vector3f& cy, const Vector3f& cz) {
            return Matrix3f({
                cx.x, cx.y, cx.z,
                cy.x, cy.y, cy.z,
                cz.x, cz.y, cz.z
            });
        }

        static constexpr Matrix3f Diagonal(float xx, float yy, float zz) {
            return Matrix3f({
                xx, 0.f, 0.f,
                0.f, yy, 0.f,
                0.f, 0.f, zz
            });
        }

        static constexpr Matrix3f Identity() {
            return Diagonal(1.f,1.f,1.f);
        }

        static constexpr Matrix3f Outer(const Vector3f& u, const Vector3f& v) {
            return Matrix3f({
                u.x * v.x, u.y * v.x, u.z * v.x,  // column 0
                u.x * v.y, u.y * v.y, u.z * v.y,  // column 1
                u.x * v.z, u.y * v.z, u.z * v.z   // column 2
            });
        }

    };

    class Matrix4f {
        public:
        constexpr Matrix4f() = default;

        constexpr explicit Matrix4f(const std::array<float, 16> &a): m(a){}

        constexpr explicit Matrix4f(const std::array<float, 16> &&a): m(a){}

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

        static constexpr Matrix4f Rotation(const Vector3f rot) noexcept {
            return Matrix4f(Quaternion::fromEuler(
                toRad(rot.x),
                toRad(rot.y),
                toRad(rot.z)
            )
            .toMatrix());
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


        constexpr float& operator()(int row, int col) noexcept {
            return m[col * 4 + row];
        }

        constexpr float operator()(int row, int col) const noexcept {
            return m[col * 4 + row];
        }

        friend constexpr Vector4f operator*(const Matrix4f& A, const Vector4f& b) noexcept {
            return {
                A(0,0)*b.x + A(0,1)*b.y + A(0,2)*b.z + A(0,3)*b.w,
                A(1,0)*b.x + A(1,1)*b.y + A(1,2)*b.z + A(1,3)*b.w,
                A(2,0)*b.x + A(2,1)*b.y + A(2,2)*b.z + A(2,3)*b.w,
                A(3,0)*b.x + A(3,1)*b.y + A(3,2)*b.z + A(3,3)*b.w
            };
        }

        constexpr float& at(const std::size_t col, const std::size_t row) noexcept {
            return m[col * 4 + row];
        }

        [[nodiscard]] constexpr float at(const std::size_t col, const std::size_t row) const noexcept {
            return m[col * 4 + row];
        }

        ~Matrix4f() = default;

        [[nodiscard]] Matrix3f toRotationMatrix() const {

            return Matrix3f({
                    m[0],  m[1],  m[2],    // Column 0
                    m[4],  m[5],  m[6],    // Column 1
                    m[8],  m[9],  m[10]    // Column 2
                });

        }

    private:
        std::array<float, 16> m;
    };

    class Matrix2f {
        std::array<float, 4> m;

    public:
        constexpr Matrix2f() : m() {}
        constexpr Matrix2f(float aa, float ab, float ba, float bb) : m{aa, ab, ba, bb} {}

        static constexpr Matrix2f Rotation(float rot) {
            return {
                std::cos(rot), -std::sin(rot),
                std::sin(rot), std::cos(rot)
            };
        }

        constexpr float operator()(int row, int col) const noexcept {
            return m[col + row * 2];
        }

        friend constexpr Vector2f operator*(const Matrix2f& A, const Vector2f& b) noexcept {
            return {
                A(0, 0) * b.x + A(0, 1) * b.y,
                A(1, 0) * b.x + A(1, 1) * b.y
            };
        }
    };


} // Math



#endif //MIXEDPROJECT_MATRIX_H