//
// Created by ertls on 17.02.2026.
//

#pragma once


namespace Obj3D {
    class Material {
        public:
        Material();
        ~Material() = default;
        void init();

        [[nodiscard]] bool hasData() const;
    };
}
