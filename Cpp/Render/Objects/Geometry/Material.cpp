//
// Created by ertls on 17.02.2026.
//

#include "Material.h"

#include <cstdio>

namespace Obj {
    Material::Material() = default;

    void Material::init() {
        puts("init material");
    }

    bool Material::hasData() const{
        return false;
    }

} // Geometry