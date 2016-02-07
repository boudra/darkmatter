#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Core/System.hpp"
#include "Core/ComponentManager.hpp"
#include "Components/PhysicsComponent.hpp"

namespace dm {

class PhysicsSystem : public System {
   public:
    PhysicsSystem() {}
    ~PhysicsSystem() {}

    void render(const GameState&);
    void update(GameState&);
};

dm_internal_register_type(PhysicsSystem)

} /* namespace dm */

#endif
