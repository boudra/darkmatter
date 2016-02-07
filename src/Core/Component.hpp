#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <cstdint>
#include <vector>

#include "Event/Event.hpp"
#include "Util/Memory.hpp"
#include "TypeId/TypeId.hpp"

#define COMPONENT(TYPE) dm_type_id(TYPE)
#define COMPONENTS(TYPE) dm::MemoryPool<TYPE>::instance()

#define dm_register_component(type)              \
    dm_internal_memory_pool(type)

namespace dm {

typedef uint32_t ComponentId;
class Entity;

struct Component {
    Component() {}
    virtual ~Component() {}

    Entity* parent;
    uint32_t id;
    uint32_t type_id;
};

} /* end dm */

#endif
