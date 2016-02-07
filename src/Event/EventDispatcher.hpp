#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include <algorithm>
#include <functional>
#include <vector>
#include <map>

#include "TypeId/Any.hpp"
#include "Event/Event.hpp"
#include "Core/Logger.hpp"

namespace dm {

class EventDispatcher {
   public:
    EventDispatcher() {}
    ~EventDispatcher() {}

   private:
};
}

#endif
