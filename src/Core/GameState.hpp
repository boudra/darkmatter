#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <string>
#include <map>
#include <vector>

#include "TypeId/Any.hpp"
#include "Event/Event.hpp"

namespace dm {

struct GameState {
    struct SubscriptionInfo {
        std::function<void(const Event&)> handler;
        Any handler_type;
    };

    using ListenerVector = std::vector<SubscriptionInfo>;

    std::string name;
    std::map<std::string, Any> data;
    std::map<std::string, ListenerVector> _listeners;

    void transition_to(const std::string& name) {
        Log::info("Transition from %s to %s", this->name, name);
        this->name = name;
    }

    bool is(const std::string& name) const { return this->name == name; }

    template <class HandlerType, class EventValueType = Event>
    size_t subscribe(EventType type, HandlerType* handler,
                     void (HandlerType::*func)(const EventValueType&)) {
        using realfunc = void (HandlerType::*)(const Event&);

        realfunc real = reinterpret_cast<realfunc>(func);

        auto r = _listeners.insert(std::make_pair(type, ListenerVector()));

        ListenerVector& handlers = r.first->second;

        handlers.push_back(SubscriptionInfo{
            std::bind(real, (handler), std::placeholders::_1), Any(handler)});

        const auto subscription_id = handlers.size() - 1;

        Log::debug("%s subscribed to %s", dm_type_name(HandlerType), type);

        return subscription_id;
    }

    template <class HandlerType, class EventValueType = Event>
    void unsubscribe(const HandlerType* handler, EventType type) {
        auto& vector = _listeners[type];
        const auto size = vector.size();
        vector.erase(std::remove_if(vector.begin(), vector.end(),
                                    [handler](const SubscriptionInfo& sub) {
                                        return sub.handler_type == handler;
                                    }),
                     vector.end());
        if (size - vector.size() > 0) {
            Log::debug("%s unsubscribed from %s", dm_type_name(HandlerType),
                       type);
        }
    }

    template <class HandlerType>
    void unsubscribe(const HandlerType* handler) {
        for (auto& handlers : _listeners) {
            unsubscribe(handler, handlers.first.c_str());
        }
    }

    template <class EventValueType>
    void send_event(const EventValueType& e) {
        for (auto& sub : _listeners[e.type]) {
            sub.handler(e);
        }
    }

    template <typename value_type>
    const value_type& get(const std::string& key) const {
        Log::debug("get %s", key);
        return data.at(key).Cast<value_type>();
    }

    template <typename value_type>
    value_type& get(const std::string& key) {
        Log::debug("get %s", key);
        return data.at(key).Cast<value_type>();
    }

    template <typename value_type>
    void set(const std::string& key, value_type& value) {
        data[key] = value;
    }

    template <typename value_type>
    void set(const std::string&& key, value_type&& value) {
        data[key] = value;
    }
};

} /* dm  */

#endif
