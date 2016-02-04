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


struct SubscriptionInfo {
    std::function<void(const Event&)> handler;
    TypeInfo handler_type;
};

using ListenerVector = std::vector<SubscriptionInfo>;

class EventDispatcher {
   public:
    EventDispatcher() {}
    ~EventDispatcher() {}

    template <class HandlerType, class EventValueType = Event>
    size_t subscribe(EventType type, HandlerType* handler,
                     void (HandlerType::*func)(const EventValueType&)) {

        using realfunc = void (HandlerType::*)(const Event&);

        realfunc real = reinterpret_cast<realfunc>(func);

        auto r = _listeners.insert(std::make_pair(type, ListenerVector()));

        ListenerVector& handlers = r.first->second;

        handlers.push_back(SubscriptionInfo{
                std::bind(real, (handler), std::placeholders::_1),
                dm_type_info(HandlerType)
        });

        const auto subscription_id = handlers.size() - 1;

        Log::debug("%s subscribed to %s", handlers[subscription_id].handler_type.name(), type);

        return subscription_id;
    }

    void unsubscribe(EventType type, unsigned int listenerId) {
    }

    template <class EventValueType>
    void send_event(const EventValueType& e) {
        for (auto& sub : _listeners[e.type]) {
            sub.handler(e);
        }
    }

   private:
    std::map<std::string, ListenerVector> _listeners;

};
}

#endif
