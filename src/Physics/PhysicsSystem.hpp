#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "Core/System.hpp"
#include "Core/ComponentManager.hpp"
#include "Components/PhysicsComponent.hpp"

namespace dm {

class PhysicsSystem : public SystemBase {
  public:

   PhysicsSystem();
   ~PhysicsSystem();

   void render(float interpolation);
   void update();

   bool initialize();

  private:

};

} /* namespace dm */

#endif
