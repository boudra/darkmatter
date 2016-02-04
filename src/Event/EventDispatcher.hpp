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

using HandlerFunction = std::function<void(const Event&)>;
using ListenerVector = std::vector<HandlerFunction>;

class EventDispatcher {
   public:
    EventDispatcher() {}
    ~EventDispatcher() {}

    template <class HandlerType, class EventValueType = Event>
    size_t subscribe(HandlerType* handler,
                     void (HandlerType::*func)(const EventValueType&),
                     EventType type) {

        using realfunc = void (HandlerType::*)(const Event&);

        realfunc real = reinterpret_cast<realfunc>(func);

        auto r = _listeners.insert(std::make_pair(type, ListenerVector()));

        ListenerVector& handlers = r.first->second;

        handlers.push_back(std::bind(real, (handler), std::placeholders::_1));

        return handlers.size() - 1;
    }

    void unsubscribe(EventType type, unsigned int listenerId) {
    }

    template <class EventValueType>
    void send_event(const EventValueType& e) {
        for (auto& handler : _listeners[e.type]) {
            handler(e);
        }
    }

   private:
    std::map<EventType, ListenerVector> _listeners;

};
}

#endif
