//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_CUBE_H
#define MIXEDPROJECT_CUBE_H
#include "../Object.h"

namespace Obj::Primitive {
    class Cube: public Object{
        public:
        Cube();

        Cube(Math::Vector2f pos, float scale);

        ~Cube() = default;

    };
}

#endif //MIXEDPROJECT_CUBE_H
