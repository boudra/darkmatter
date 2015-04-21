#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <cstdint>
#include <map>

#include "Util/Memory.hpp"
#include "Core/Component.hpp"

namespace dm {

  class Entity {

  public:

    MEMORY_POOL(Entity)

    Entity() {}

    template <class ComponentType>
    ComponentType& component() {
      assert(has_component<ComponentType>());
      return *(static_cast<ComponentType*>(components[COMPONENT(ComponentType)]));
    }

    template <class ComponentType>
    ComponentType& assign() {
      ComponentType * component = new ComponentType();
      components[COMPONENT(ComponentType)] = static_cast<ComponentBase*>(component);
      component->parent = this; 
      return * component;
    }

    template <class ComponentType>
    void remove_component() {
      assert(has_component<ComponentType>());
      components.erase(COMPONENT(ComponentType));
    }

    template <class ComponentType>
    bool has_component() const {
      return components.find(COMPONENT(ComponentType)) != components.end();
    }

    void reset() {
    }

    bool is_valid() {
      assert(false);
      return _valid;
    }

  private:

    std::map<uint32_t, ComponentBase*> components;
    
    bool _valid;

  };

  dm_internal_register_type_name(Entity, "Entity");

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
