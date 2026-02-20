//
// Created by ertls on 17.02.2026.
//

#ifndef MIXEDPROJECT_MATERIAL_H
#define MIXEDPROJECT_MATERIAL_H

namespace Obj {
    class Material {
        public:
        Material();
        ~Material() = default;
        void init();

        bool hasData() const;
    };
} // Geometry

#endif //MIXEDPROJECT_MATERIAL_H