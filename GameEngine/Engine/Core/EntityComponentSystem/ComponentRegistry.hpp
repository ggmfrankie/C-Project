//
// Created by Stefan on 12.10.2026.
//

#pragma once
#include <bitset>
#include <deque>
#include <iostream>
#include <optional>
#include <span>
#include <variant>

#include "Entity.hpp"
#include "DataStorage/HashMap.hpp"

namespace ggm {
    template<typename T, typename... Ts>
        concept ContainsType = (std::is_same_v<T, Ts> || ...);

    template<typename... Ts>
        struct TypeList {};


    template<typename... Components>
    class ComponentRegistry {
        using ComponentVec = std::variant<std::vector<Components>...>;
        using ComponentId = i16;

        struct TypeMapping {
            std::array<ComponentId, sizeof...(Components)> mappings;

            ComponentId& operator[](size_t index) {
                return mappings[index];
            }

            const ComponentId& operator[](size_t index) const {
                return mappings[index];
            }

            bool operator==(const TypeMapping& other) const {
                return other.mappings == mappings;
            }

            size_t size() {
                return mappings.size();
            }
        };

        struct TypeSignature {
            std::bitset<sizeof...(Components)> signature;

            template<typename... RemovedComponents>
            requires (ContainsType<RemovedComponents, Components...> && ...)
            TypeSignature removeTypes() {
                ((signature[componentId<RemovedComponents>()] = false), ...);
                return *this;
            }

            template<typename... AddedComponents>
            requires (ContainsType<AddedComponents, Components...> && ...)
            TypeSignature addTypes() {
                ((signature[componentId<AddedComponents>()] = true), ...);
                return *this;
            }

            bool operator==(const TypeSignature& other) const {
                return other.signature == signature;
            }

            size_t size() const {
                return signature.size();
            }

            TypeSignature operator&(const TypeSignature& other) {
                return {other.signature & signature};
            }

            bool operator[](size_t index) const {
                return signature[index];
            }

            void set(size_t index, bool value) {
                signature.set(index, value);
            }
        };

        template<typename T>
        static consteval ComponentId componentId() {
            constexpr bool matches[] = {
                std::is_same_v<T, Components>...
            };

            for (ComponentId i = 0; i < sizeof(matches); ++i) {
                if (matches[i]) return i;
            }

            throw "Component not found";
        }

    public:
        class Archetype  {
            template<typename T>
            static void setMapping(TypeMapping& mapping, ComponentId index) {
                mapping[componentId<T>()] = index;
            }

            template<typename... UsedComponents, std::size_t... Ids>
            requires (ContainsType<UsedComponents, Components...> && ...)
            static void setMappings(TypeMapping& mapping, std::index_sequence<Ids...>) {
                (setMapping<UsedComponents>(mapping, Ids), ...);
            }

            static void setMappings(TypeMapping& mapping, const TypeSignature& signature) {
                for (int i = 0, index = 0; i < signature.size(); ++i) {
                    if (signature[i]) {
                        mapping[i] = index++;
                    }
                }
            }

            bool has(std::span<const ComponentId> ids) const {
                for (auto id : ids) {
                    if (mMappings[id] == EMPTY_MAPPING) return false;
                }
                return true;
            }

            void updateSignature() {
                for (int i = 0; i < mMappings.size(); ++i) {
                    mSignature.set(i, mMappings[i] != EMPTY_MAPPING);
                }
            }

            TypeMapping mMappings;
            TypeSignature mSignature;
            std::vector<ComponentVec> mComponents;
            std::vector<Entity> mEntities;

            HashMap<decltype(Entity::mId), size_t> mEntityMappings;


            static constexpr i16 EMPTY_MAPPING = -1;

            Archetype() {
                std::ranges::fill(mMappings.mappings, EMPTY_MAPPING);
            }

            [[nodiscard]] TypeMapping& getMappings() {
                return mMappings;
            }

            [[nodiscard]] std::vector<ComponentVec>& getComponents() {
                return mComponents;
            }

        public:
            [[nodiscard]] size_t getEntityCount() {
                return mEntityMappings.size();
            }

            [[nodiscard]] TypeSignature getSignature() {
                return mSignature;
            }

            [[nodiscard]] HashMap<decltype(Entity::mId), size_t>& getEntityMappings() {
                return mEntityMappings;
            }

            [[nodiscard]] std::vector<Entity>& getEntities() {
                return mEntities;
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

                out.updateSignature();
                return out;
            }

            static Archetype New(TypeSignature& signature) {
                Archetype out {};
                auto& components = out.getComponents();
                components.reserve(signature.size());

                [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    (
                        [&] {
                            if (signature[Is]) {
                                using T = std::tuple_element_t<Is, std::tuple<Components...>>;
                                components.emplace_back(std::in_place_type<std::vector<T>>);
                            }
                        }(),
                        ...
                    );
                }(std::index_sequence_for<Components...>{});

                // Make this a member
                setMappings(out.getMappings(), signature);
                out.updateSignature();
                return out;
            }

            template<typename UsedComponent>
            std::vector<UsedComponent>& getComponentVector() {
                return std::get<std::vector<std::remove_cvref_t<UsedComponent>>>(mComponents[mMappings[componentId<std::remove_cvref_t<UsedComponent>>()]]);
            }

            ComponentVec& getComponentVector(ComponentId id) {
                return mComponents[mMappings[id]];
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
            void add(Entity&& entity, UsedComponents&&... args) {
                mEntityMappings.insert(entity.mId, entity.mId);
                mEntities.push_back(std::move(entity));

                (getComponentVector<UsedComponents>().push_back(
                    std::forward<UsedComponents>(args)), ...);
            }

            bool contains(const Entity& entity) {
                return mEntityMappings.contains(entity.mId);
            }

            std::optional<size_t> getIndex(const Entity& entity) {
                if (size_t* index = mEntityMappings[entity.mId]) {
                    return *index;
                }
                return std::nullopt;
            }

            bool hasTypeSignature(const TypeSignature& signature) {
                return mSignature == signature;
            }

            ~Archetype() = default;
        };

    private:
        std::deque<Archetype> mArchetypes;

    public:
        template<typename... UsedComponents>
        requires (ContainsType<UsedComponents, Components...> && ...)
        void createEntity(Entity&& entity, UsedComponents&&... args) {
            // Use static initialization to create a new archetype or use old one if archetype already exists
            static int index = -1;
            if (index == -1) {
                mArchetypes.push_back(Archetype::template New<std::remove_cvref_t<UsedComponents>...>());
                index = mArchetypes.size()-1;
            }

            mArchetypes[index].add(std::move(entity), std::forward<UsedComponents>(args)...);
        }

        template<typename... RemovedComponents, typename... AddedComponents>
        requires (ContainsType<RemovedComponents, Components...> && ...) &&  (ContainsType<AddedComponents, Components...> && ...)
        void changeComponents(const Entity& entity, Archetype& sourceArchetype, TypeList<RemovedComponents...> removed, AddedComponents&&... added) {
            Archetype* destinationArchetype {};

            auto it = sourceArchetype.getIndex(entity);
            assert(it);
            ssize_t index = *it;

            // Calculate the required new Archetype signature
            TypeSignature present = sourceArchetype.getSignature();
            TypeSignature required = present.template removeTypes<std::remove_cvref_t<RemovedComponents>...>().template addTypes<std::remove_cvref_t<AddedComponents>...>();
            TypeSignature canBeTransferred = present & required;

            // Search existing Archetypes
            for (auto& a : mArchetypes) {
                if (a.hasTypeSignature(required)) {
                    destinationArchetype = &a;
                    break;
                }
            }

            // Create new Archetype if necessary
            if (destinationArchetype == nullptr) {
                mArchetypes.push_back(Archetype::New(required));
                destinationArchetype = &mArchetypes.back();
            }

            assert(destinationArchetype != nullptr);

            for (int i = 0; i < required.size(); ++i) {
                if (!required[i]) continue;
                if (canBeTransferred[i]) {
                    std::visit([&]<typename Type>(Type& sourceVector) {
                        using Vector = std::remove_cvref_t<Type>;
                        using T = Vector::value_type;

                        auto& destinationVector = destinationArchetype->template getComponentVector<T>();

                        destinationVector.emplace_back(
                            std::move(sourceVector[index])
                        );
                    }, sourceArchetype.getComponentVector(i));

                    destinationArchetype->getEntities().emplace_back(
                        std::move(sourceArchetype.getEntities()[index])
                    );
                } else {
                    (
                        [&] {
                            using T = std::remove_cvref_t<AddedComponents>;

                            if (componentId<T>() != i) return false;

                            auto& destinationVector = destinationArchetype->template getComponentVector<T>();
                            destinationVector.emplace_back(added);

                            return true;
                        }() || ...
                    );
                }
            }

            for (int i = 0; i < present.size(); ++i) {
                if (!present[i]) continue;
                std::visit([&]<typename Type>(Type& sourceVector) {
                    if (size_t lastIndex = sourceVector.size()-1; index != lastIndex) {
                        sourceVector[index] = std::move(sourceVector.back());
                        sourceArchetype.getEntityMappings().find(lastIndex)->value = index;
                    }
                    sourceVector.pop_back();
                }, sourceArchetype.getComponentVector(i));

                std::vector<Entity>& sourceEntities = sourceArchetype.getEntities();
                if (const size_t lastIndex = sourceEntities.size()-1; index != lastIndex) sourceEntities[index] = std::move(sourceEntities.back());
                sourceEntities.pop_back();
            }
        }

        template<typename... RemovedComponents, typename... AddedComponents>
        requires (ContainsType<RemovedComponents, Components...> && ...) &&  (ContainsType<AddedComponents, Components...> && ...)
        void changeComponents(const Entity& entity, TypeList<RemovedComponents...> removed, AddedComponents&&... added) {
            // Find the Archetype the entity is inside
            for (auto& a : mArchetypes) {
                if (a.contains(entity)) {
                    changeComponents(entity, a, removed, std::forward<AddedComponents>(added)...);
                    return;
                }
            }
        }

        template<typename... UsedComponents>
        requires (ContainsType<UsedComponents, Components...> && ...)
        auto& getArchetypes() {
            using ComponentBatch = std::tuple<std::size_t, std::vector<Entity>&, std::vector<UsedComponents>&...>;

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
                    archetype.getEntities(),
                    archetype.template getComponentVector<UsedComponents>()...
                );
            }

            return batches;
        }
    };
} // ggm
