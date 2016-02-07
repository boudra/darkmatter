#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <cstdint>

#include <vector>
#include <initializer_list>
#include <string>

#include "Core/Entity.hpp"
#include "Core/GameState.hpp"

namespace dm {

class EntityManager;
class Engine;
class EventDispatcher;

class System {
   public:
    System() : _enabled(false) {}
    virtual ~System() {}

    const bool enabled() const { return _enabled; }

    void disable() { _enabled = false; }
    void enable() { _enabled = true; }

    virtual void update(GameState&) = 0;
    virtual void render(const GameState&) = 0;

   private:
    bool _enabled;
};
}

#endif
