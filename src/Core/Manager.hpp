#ifndef MANAGER_HPP
#define MANAGER_HPP

#include "Core/Logger.hpp"
#include "Core/GameState.hpp"

#include <cstdint>

namespace dm {

class EventDispatcher;
class Engine;

typedef uint32_t ManagerId;

class ManagerBase {
   public:
    virtual ~ManagerBase(){};

   protected:
    friend class Engine;

    static EventDispatcher* s_dispatcher;
    static Engine* s_engine;

    static ManagerId& counter() {
        static ManagerId counter = 0;
        return counter;
    }
};

template <typename DerivedType>
class Manager : public ManagerBase {
   public:
    Manager(){};
    virtual ~Manager(){};
    virtual const bool initialize(GameState& state) { return true; }

    static ManagerId id() {
        static ManagerId id = counter()++;
        return id;
    }
};
}

#endif
