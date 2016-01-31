#ifndef EVENT_SYSTEM_HPP
#define EVENT_SYSTEM_HPP

#include <algorithm>
#include <functional>
#include <vector>

#include "TypeId/Any.hpp"
#include "Event/Event.hpp"
#include "Core/Logger.hpp"

namespace dm {

typedef std::function<void(const Event&)> ListenerFunc;

using ListenerVector = std::vector<std::function<void(const Event&)>>;

class EventDispatcher {
   public:
    EventDispatcher() { m_listeners.resize(EventType::NUM_EVENTS); }
    ~EventDispatcher() {}

    template <class HandlerType, class EventValueType = Event>
    size_t subscribe(HandlerType* handler,
                     void (HandlerType::*func)(const EventValueType&),
                     EventType type) {
        using realfunc = void (HandlerType::*)(const Event&);

        realfunc real = reinterpret_cast<realfunc>(func);

        if (m_listeners.size() <= type || m_handlers.size() <= type) {
            m_listeners.resize(type + 1);
            m_handlers.resize(type + 1);
        }

        m_listeners[type].push_back(
            std::bind(real, (handler), std::placeholders::_1));

        /* TODO FIX COUNTER */
        size_t subscribe_id = m_listeners[type].size() - 1;

        m_handlers[type].push_back(
            std::make_pair(reinterpret_cast<uintptr_t>(handler), subscribe_id));

        return subscribe_id;
    }

    void unsubscribe(EventType type, unsigned int listenerId) {
        /*
        auto& listeners =
        m_listeners[e.Type].cast<std::vector<std::function<void
        (const EventValueType&)>>>();
        listeners.erase(listeners.begin() + listenerId);çç*/
    }

    template <class EventValueType>
    void send_event(const EventValueType& e) {
        for (auto& handler : m_listeners[e.type]) {
            handler(e);
        }
    }

   private:
    std::vector<ListenerVector> m_listeners;
    std::vector<std::vector<std::pair<uintptr_t, size_t>>> m_handlers;
};
}

#endif
