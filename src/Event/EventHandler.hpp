#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

#include "EventDispatcher.hpp"

#include <map>
#include <cstdint>

namespace dm {


/*
template <class DerivedType>
class EventHandler {
  public:

   void UnsubscribeFromAllEvents() {
      for (const auto& i : m_subscribed) {
         this->m_dispatcher->Unsubscribe(i.first, i.second);
      }
      m_subscribed.clear();
   }

   template <class EventValueType = Event>
   void Subscribe(void (DerivedType::*func)(const EventValueType&),
                  EventType type) {
      uint32_t id =
          m_dispatcher->Subscribe(static_cast<DerivedType*>(this), func, type);
         m_subscribed.insert(std::make_pair(type, id));
   }

  private:
   std::map<EventType, uint32_t> m_subscribed;

  protected:
   static EventDispatcher * m_dispatcher;
   ~EventHandler() { this->UnsubscribeFromAllEvents(); }
};
*/
}

#endif
