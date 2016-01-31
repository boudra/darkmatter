#include "Entity.hpp"
#include "Serialize/Serializer.hpp"

namespace dm {

bool Entity::load_from(const char* file) {
    Deserializer des;
    return des.deserialize(file, *this);
}
}
