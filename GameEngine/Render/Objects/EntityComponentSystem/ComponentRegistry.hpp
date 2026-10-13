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

        public:
            [[nodiscard]] auto& getMappings() const {
                return mMappings;
            }

            [[nodiscard]] auto& getComponents() const {
                return mComponents;
            }

            template<typename... UsedComponents>
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

            template<typename... UsedComponents>
            bool has() const {
                constexpr std::array ids {
                    componentId<UsedComponents>()...
                };
                return has(std::span<const ComponentId>{ids});
            }

            template<typename... UsedComponents>
            void add(Entity& entity, UsedComponents&&... args) {
                mEntities.insert(entity.mId, 43);
            }

            ~Archetype() = default;
        };

    private:
        std::vector<Archetype> mArchetypes;

    public:
        template<typename... UsedComponents>
        ComponentRegistry& add(Entity entity, UsedComponents... args) {
            // Use static initialization to create a new archetype and use old one if archetype already exists
            static int index = -1;
            if (index == -1) index = [this]{mArchetypes.push_back(Archetype::template New<UsedComponents...>()); return mArchetypes.size()-1;}();

            mArchetypes[index].add(entity, std::forward<UsedComponents>(args)...);
        }
    };

    static void test() {
        struct Sprite {
            Vec2f dims;
        };

        struct Position: Vec2f {
            using Vec2f::Vec2f;
        };

        struct Velocity: Vec2f {
            using Vec2f::Vec2f;
        };

        struct Health {
            int amount;
        };

        ComponentRegistry<Sprite, Position, Velocity, Health> registry {};
    }
} // ggm
