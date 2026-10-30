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
#include <format>
#include <sstream>

#include "Entity.hpp"
#include "DataStorage/HashMap.hpp"
#include "Engine/Debug/Logger.hpp"

namespace ggm {
    template<typename T, typename... Ts>
        concept ContainsType = (std::is_same_v<T, Ts> || ...);

    template<typename... Ts>
        struct TypeList {};

    template<typename... Components>
    class ComponentRegistry {
        using ComponentVec = std::variant<std::vector<Components>...>;
        using ComponentId = i16;

        using TypeMapping = std::array<ComponentId, sizeof...(Components)>;

        struct TypeSignature {
            static std::tuple<Components...> types;

            std::bitset<sizeof...(Components)> bits;

            template<typename... RemovedComponents>
            requires (ContainsType<RemovedComponents, Components...> && ...)
            TypeSignature removeTypes() const {
                std::bitset<sizeof...(Components)> newBits = bits;
                ((newBits.set(componentId<RemovedComponents>(), false)), ...);
                return {newBits};
            }

            template<typename... AddedComponents>
            requires (ContainsType<AddedComponents, Components...> && ...)
            TypeSignature addTypes() const {
                std::bitset<sizeof...(Components)> newBits = bits;
                ((newBits.set(componentId<AddedComponents>(), true)), ...);
                return {newBits};
            }

            bool operator==(const TypeSignature& other) const {
                return other.bits == bits;
            }

            size_t size() const {
                return bits.size();
            }

            TypeSignature operator&(const TypeSignature& other) const {
                return {other.bits & bits};
            }

            bool operator[](size_t index) const {
                return bits[index];
            }

            void set(size_t index, bool value) {
                bits.set(index, value);
            }

            std::string getTypeNames() {
                std::ostringstream out;
                (
                    [&] {
                        constexpr ComponentId id = componentId<Components>();
                        using T = std::tuple_element_t<id, decltype(types)>;
                        if (bits[id]) out << typeid(T).name() << ' ';
                    }(), ...
                );

                return out.str();
            }

            friend std::ostream& operator<<(std::ostream& os, const TypeSignature& ts) {
                os << ts.bits;
                return os;
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
                    mapping[i] = (signature[i]) ? index++ : EMPTY_MAPPING;
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
                std::ranges::fill(mMappings, EMPTY_MAPPING);
            }

            [[nodiscard]] std::vector<ComponentVec>& getComponents() {
                return mComponents;
            }

        public:
            [[nodiscard]] size_t getEntityCount() const {
                assert(mEntities.size() == mEntityMappings.size());
                return mEntities.size();
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

            [[nodiscard]] TypeMapping& getMappings() {
                return mMappings;
            }

            Archetype(const Archetype&) = delete;
            Archetype& operator=(const Archetype&) = delete;

            Archetype(Archetype&&) noexcept = default;
            Archetype& operator=(Archetype&&) noexcept = default;

            template<typename... UsedComponents>
            requires (ContainsType<UsedComponents, Components...> && ...)
            static Archetype New() {
                Archetype out {};

                auto& components = out.getComponents();
                components.reserve(sizeof...(UsedComponents));

                // Add all the needed vectors that are required for this archetype
                (components.emplace_back(
                    std::in_place_type<std::vector<std::remove_cvref_t<UsedComponents>>>
                ), ...);

                // Adjust the mappings
                setMappings<std::remove_cvref_t<UsedComponents>...>(out.getMappings(), std::index_sequence_for<UsedComponents...>{});

                out.updateSignature();

                Logger::info({"Created new Archetype with signature {} and types\n [ {}]", out.getSignature().bits, out.getSignature().getTypeNames()});
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

                Logger::info({"Created new Archetype from signature {} and types\n [ {}]", signature.bits, out.getSignature().getTypeNames()});
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
            void add(Entity entity, UsedComponents&&... args) {
                mEntityMappings.insert(entity.mId, mEntities.size());
                mEntities.push_back(entity);

                (getComponentVector<std::remove_cvref_t<UsedComponents>>().emplace_back(
                    std::forward<UsedComponents>(args)), ...);
            }

            void remove(Entity e) {
                const auto it = getIndex(e);
                assert(it);

                const ssize_t index = *it;
                const ssize_t lastIndex = mEntities.size()-1;

                if (index != lastIndex) {
                    mEntities[index] = std::move(mEntities.back());

                    *mEntityMappings[mEntities[index].mId] = index;
                }

                // Delete all components
                for (auto& component : mComponents) {
                    std::visit([&]<typename Type>(Type& sourceVector) {
                        if (index != lastIndex) {
                            sourceVector[index] = std::move(sourceVector.back());
                        }
                        sourceVector.pop_back();
                    }, component);
                }

                mEntities.pop_back();
                mEntityMappings.remove(e.mId);
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

        Archetype& getArchetype(const Entity& entity) {
            for (auto& a : mArchetypes) {
                if (a.contains(entity)) return a;
            }
            Logger::error({"Entity not found"});
            return {};
        }

    public:
        template<typename... UsedComponents>
        requires (ContainsType<UsedComponents, Components...> && ...)
        void createEntity(Entity entity, UsedComponents&&... args) {
            // Use static initialization to create a new archetype or use old one if archetype already exists
            static int index = -1;
            if (index == -1) {
                mArchetypes.push_back(Archetype::template New<std::remove_cvref_t<UsedComponents>...>());
                index = mArchetypes.size()-1;
            }

            mArchetypes[index].add(entity, std::forward<UsedComponents>(args)...);
        }

        void deleteEntity(Entity entity) {
            Archetype& archetype = getArchetype(entity);

            archetype.remove(entity);
        }

        Archetype& getOrCreateArchetype(TypeSignature signature) {
            for (auto& archetype : mArchetypes) {
                if (archetype.hasTypeSignature(signature)) return archetype;
            }

            mArchetypes.push_back(Archetype::New(signature));
            return mArchetypes.back();
        }

        template<typename... RemovedComponents, typename... AddedComponents>
        requires (ContainsType<RemovedComponents, Components...> && ...) &&  (ContainsType<AddedComponents, Components...> && ...)
        void changeComponentsV2(Entity entity, Archetype& sourceArchetype, TypeList<RemovedComponents...> removed, AddedComponents&&... added) {
            auto it = sourceArchetype.getIndex(entity);
            assert(it);

            assert(sourceArchetype.template has<RemovedComponents...>());

            ssize_t changedEntityIndex = *it;
            (void) changedEntityIndex;

            struct {
                TypeSignature present;
                TypeSignature required;
                TypeSignature transferable;
            } signatures = {};

            signatures.present      = sourceArchetype.getSignature();
            signatures.required     = signatures.present.template removeTypes<RemovedComponents...>().template addTypes<AddedComponents...>();
            signatures.transferable = signatures.present & signatures.required.template removeTypes<AddedComponents...>();

            Archetype& destinationArchetype = getOrCreateArchetype(signatures.required);

            std::tuple<AddedComponents...> addedComponents {std::forward<AddedComponents>(added)...};

            // Adding of the components to the destinationVector
            for (int i = 0; i < signatures.required.size(); ++i) {
                if (!signatures.required[i]) continue;

                if (signatures.transferable[i]) {
                    // Transfer the component from the sourceVector to the destinationVector
                    std::visit([&]<typename Vector>(Vector& sourceVector) {
                        using T = Vector::value_type;

                        std::vector<T>& destinationVector = destinationArchetype.template getComponentVector<T>();

                        destinationVector.push_back(
                            std::move(sourceVector[changedEntityIndex])
                        );

                    }, sourceArchetype.getComponentVector(i));
                } else {
                    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                        (
                        [&]<std::size_t I>() {
                            using T = std::tuple_element_t<I, decltype(addedComponents)>;

                            if (componentId<T>() != i) return false;

                            auto& destinationVector = destinationArchetype.template getComponentVector<T>();
                            destinationVector.emplace_back(std::move(std::get<Is>(addedComponents)));

                            return true;
                        }.template operator()<Is>() || ...
                        );
                    }(std::index_sequence_for<AddedComponents...>{});
                }
            }

            {
                auto& destinationMappings = destinationArchetype.getEntityMappings();
                auto& destinationEntities = destinationArchetype.getEntities();

                destinationMappings.insert(entity.mId, destinationEntities.size());
                destinationEntities.push_back(entity);
            }

            // Removing of the components from the sourceVector
            sourceArchetype.remove(entity);
        }

        template<typename... RemovedComponents, typename... AddedComponents>
        requires (ContainsType<RemovedComponents, Components...> && ...) &&  (ContainsType<AddedComponents, Components...> && ...)
        void changeComponents(Entity entity, TypeList<RemovedComponents...> removed, AddedComponents&&... added) {
            // Find the Archetype the entity is inside
            for (auto& a : mArchetypes) {
                if (a.contains(entity)) {
                    changeComponentsV2(entity, a, removed, std::forward<AddedComponents>(added)...);
                    return;
                }
            }
        }

        template<typename... UsedComponents>
        requires (ContainsType<UsedComponents, Components...> && ...)
        auto& getArchetypes() {
            using ComponentBatch = std::tuple<std::size_t, std::vector<Entity>&, std::vector<UsedComponents>&...>;

            static std::vector<ComponentBatch> batches {};
            batches.clear();

            for (auto& archetype : mArchetypes) {
                if (!archetype.template has<UsedComponents...>()) continue;
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

template<std::size_t N>
struct std::formatter<std::bitset<N>> : std::formatter<std::string_view> {
    auto format(const std::bitset<N>& bits, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", bits.to_string());
    }
};