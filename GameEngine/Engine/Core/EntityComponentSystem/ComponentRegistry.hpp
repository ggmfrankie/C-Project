//
// Created by Stefan on 12.10.2026.
//

#pragma once
#include <span>
#include <variant>

#include "Entity.hpp"
#include "DataStorage/HashMap.hpp"
#include "Utils/Math/Vector.h"

namespace ggm {
    template<typename T, typename... Ts>
        concept ContainsType = (std::is_same_v<T, Ts> || ...);

    template<typename... Components>
    class ComponentRegistry {
        using Component = std::variant<std::vector<Components>...>;
        using ComponentId = i16;

        using TypeMapping = std::array<ComponentId, sizeof...(Components)>;

        template<typename T>
            static consteval ComponentId componentId() {
            constexpr bool matches[] = {
                std::is_same_v<T, Components>...
            };

            for (u8 i = 0; i < sizeof(matches); ++i) {
                if (matches[i]) return i;
            }

            throw "Component not found";
        }
    public:
        class Archetype  {
            template<typename T>
            static void setMapping(TypeMapping& mapping, u8 index) {
                mapping[componentId<T>()] = index;
            }

            template<typename... UsedComponents, std::size_t... Ids>
            requires (ContainsType<UsedComponents, Components...> && ...)
            static void setMappings(TypeMapping& mapping, std::index_sequence<Ids...>) {
                (setMapping<UsedComponents>(mapping, Ids), ...);
            }

            bool has(std::span<const ComponentId> ids) const  {
                for (auto id : ids) {
                    if (mMappings[id] == EMPTY_MAPPING) return false;
                }
                return true;
            }

            TypeMapping mMappings;
            std::vector<Component> mComponents;
            std::vector<Entity> mObjects;

            HashMap<decltype(Entity::mId), size_t> mEntities;

            static constexpr i16 EMPTY_MAPPING = -1;

            Archetype() {
                std::ranges::fill(mMappings, EMPTY_MAPPING);
            }

            [[nodiscard]] TypeMapping& getMappings() {
                return mMappings;
            }

            [[nodiscard]] std::vector<Component>& getComponents() {
                return mComponents;
            }

        public:
            [[nodiscard]] size_t getEntityCount() {
                return mEntities.size();
            }

            template<typename... UsedComponents>
            requires (ContainsType<UsedComponents, Components...> && ...)
            static Archetype New() {
                Archetype out {};
                auto& components = out.getComponents();
                components.reserve(sizeof...(UsedComponents));

                // Add all the needed vectors that are required for this archetype
                (components.emplace_back(
                    std::in_place_type<std::vector<UsedComponents>>
                ), ...);

                // Adjust the mappings
                setMappings<UsedComponents...>(out.getMappings(), std::index_sequence_for<UsedComponents...>{});
                return out;
            }

            template<typename UsedComponent>
            std::vector<UsedComponent>& getComponentVector() {
                return std::get<std::vector<std::remove_cvref_t<UsedComponent>>>(mComponents[mMappings[componentId<std::remove_cvref_t<UsedComponent>>()]]);
            }

            template<typename... UsedComponents>
            requires (ContainsType<UsedComponents, Components...> && ...)
            bool has() const {
                constexpr std::array ids {
                    componentId<UsedComponents>()...
                };
                return has(std::span<const ComponentId>{ids});
            }

            template<typename... UsedComponents>
            requires (ContainsType<UsedComponents, Components...> && ...)
            void add(Entity& entity, UsedComponents&&... args) {
                mEntities.insert(entity.mId, entity.mId);

                (getComponentVector<UsedComponents>().push_back(
                    std::forward<UsedComponents>(args)), ...);
            }

            ~Archetype() = default;
        };

    private:
        std::vector<Archetype> mArchetypes;

    public:
        template<typename... UsedComponents>
        requires (ContainsType<UsedComponents, Components...> && ...)
        void createEntity(Entity entity, UsedComponents&&... args) {
            // Use static initialization to create a new archetype or use old one if archetype already exists
            static int index = -1;
            if (index == -1) index = [this]{mArchetypes.push_back(Archetype::template New<std::remove_cvref_t<UsedComponents>...>()); return mArchetypes.size()-1;}();

            mArchetypes[index].add(entity, std::forward<UsedComponents>(args)...);
        }

        template<typename... UsedComponents>
        requires (ContainsType<UsedComponents, Components...> && ...)
        auto& getArchetypes() {
            using ComponentBatch = std::tuple<std::size_t, std::vector<UsedComponents>&...>;

            static std::vector<int> indices {};
            static std::vector<ComponentBatch> batches {};
            if (indices.empty()) {
                for (size_t i = 0; i < mArchetypes.size(); ++i) {
                    if (mArchetypes[i].template has<UsedComponents...>()) indices.push_back(i);
                }
            }

            batches.clear();

            for (int index : indices) {
                Archetype& archetype = mArchetypes[index];

                batches.emplace_back(
                    archetype.getEntityCount(),
                    archetype.template getComponentVector<UsedComponents>()...
                );
            }

            return batches;
        }
    };
} // ggm
