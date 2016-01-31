#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <cstdint>
#include <vector>

#include "Event/Event.hpp"
#include "Util/Memory.hpp"
#include "TypeId/TypeId.hpp"

#define COMPONENT(TYPE) dm_type_id(TYPE)
#define COMPONENTS(TYPE) dm::MemoryPool<TYPE>::instance()

#define dm_start_component(type)         \
    struct type : public ComponentBase { \
    MEMORY_POOL(type)

#define dm_attr(type, name) type name

#define dm_end_component(type) \
    }                          \
    ;                          \
    dm_internal_register_type_name(type, #type);

namespace dm {

typedef uint32_t ComponentId;
class Entity;

struct ComponentBase {
    ComponentBase() {}
    virtual ~ComponentBase() {}

    Entity *parent;
    uint32_t id;
    uint32_t type_id;
};

} /* end dm */

#endif
