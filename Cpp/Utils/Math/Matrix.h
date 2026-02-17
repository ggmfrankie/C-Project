//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_MATRIX_H
#define MIXEDPROJECT_MATRIX_H

namespace Math {
    class Matrix4f {
        public:
        Matrix4f();
        ~Matrix4f() = default;

        private:
        float content[4][4]{};
    };
} // Math

#endif //MIXEDPROJECT_MATRIX_H