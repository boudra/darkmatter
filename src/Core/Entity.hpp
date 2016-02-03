#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <map>

#include "Util/Memory.hpp"
#include "Core/Component.hpp"

namespace dm {

class Entity {
   public:
    Entity() {}

    template <class ComponentType>
    ComponentType& component() {
        assert(has_component<ComponentType>(), "Entity doesn't have component");
        return *(
            static_cast<ComponentType*>(components[COMPONENT(ComponentType)]));
    }

    template <class ComponentType>
    ComponentType& assign() {
        ComponentType* component = new ComponentType();
        components[COMPONENT(ComponentType)] =
            static_cast<Component*>(component);
        component->parent = this;
        return *component;
    }

    template <class ComponentType>
    void remove_component() {
        assert(has_component<ComponentType>(), "Entity doesn't have component");
        components.erase(COMPONENT(ComponentType));
    }

    template <class ComponentType>
    bool has_component() const {
        return components.find(COMPONENT(ComponentType)) != components.end();
    }

    bool load_from(const char*);

    void reset() {}

    dm_memory_pool_impl(Entity);

   private:
    std::map<uint32_t, Component*> components;
};

dm_internal_register_type(Entity)

    /* Event defining an added component */
    struct ComponentAddedEvent : Event {
    ComponentAddedEvent(const uint32_t type, const Entity* entity)
        : Event(EventType::COMPONENT_ADDED),
          component_type(type),
          entity(entity) {}

    const uint32_t component_type;
    const Entity* entity;
};

/* Event defining a removed component */
struct ComponentRemovedEvent : Event {
    ComponentRemovedEvent(const uint32_t type, const Entity* entity)
        : Event(EventType::COMPONENT_REMOVED),
          component_type(type),
          entity(entity) {}

    const uint32_t component_type;
    const Entity* entity;
};

} /* namespace dm */

#endif /* end of include guard: ENTITY_HPP */
