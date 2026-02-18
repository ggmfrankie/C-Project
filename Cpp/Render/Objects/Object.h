//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_OBJECT_H
#define MIXEDPROJECT_OBJECT_H
#include <vector>

#include "Geometry/Mesh.h"

namespace Obj {
    class Object {
        public:

        explicit Object();

        explicit Object(const std::string &objFile);

        ~Object();

        void init();

        void render() const;
        private:
        bool initialized = false;
        std::vector<Geometry::Mesh> meshes;
    };
} // Core

#endif //MIXEDPROJECT_OBJECT_H