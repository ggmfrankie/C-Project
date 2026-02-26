//
// Created by ertls on 26.02.2026.
//

#ifndef MIXEDPROJECT_GAMEINTERFACE_H
#define MIXEDPROJECT_GAMEINTERFACE_H
#include <vector>

#include "../Render/IO/Input.h"
#include "../Render/Objects/Object.h"
#include "../Render/Objects/Loader/ObjLoader.h"
#include "../Render/Transformation/Camera.h"


namespace Render {

    struct EngineContext {
        Input& input;
        Camera& camera;
        std::vector<Obj::Object>& objects;
    };

    class IGame {
    public:
        virtual ~IGame() = default;

        virtual void onInit() = 0;

        virtual void onShutdown() = 0;

        virtual void onUpdate(float deltaTime) = 0;

        virtual void passState(EngineContext&& e);
    };




} // ggm

#endif //MIXEDPROJECT_GAMEINTERFACE_H