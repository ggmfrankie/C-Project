//
// Created by Stefan on 29.03.2026.
//

#pragma once
#include "Games/IGameLayer.hpp"
#include "Render/Objects/Objects2D/Render/RenderObject2D.hpp"
#include "Render/Shader/Shader.hpp"
#include "Utils/DataStorage/SparseSet.hpp"

namespace Render {
    class Input;
}

namespace Game {
    class RenderLayer2D final : public IGameLayer  {
        Render::Shader mShader;
        Render::Input* mInput = nullptr;

        ggm::SparseSet<Obj2D::RenderObject2D> mObjects;

    public:
        RenderLayer2D();
        ~RenderLayer2D() override;

        void onAttach(const LayerEngineContext &ec) override;
        void onDetach() override;

        void onInit() override;

        void onUpdate(float dt) override;
        void onRender(int width, int height) override;


        template<typename... Args>
        auto newObject(Args &&... args) -> std::pair<ggm::SparseSet<Obj2D::RenderObject2D>&, ggm::i64> {
            return { mObjects, static_cast<ggm::u64>(mObjects.push(std::forward<Args>(args)...)) };
        }
    };
} // Game