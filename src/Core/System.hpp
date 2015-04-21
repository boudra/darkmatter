#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <cstdint>

#include <vector>
#include <initializer_list>
#include <string>

#include "Core/Entity.hpp"

#define SYSTEM(TYPE) \
   dm::SystemId<TYPE>::id()

namespace dm {

class EntityManager;
class Engine;
class EventDispatcher;

class SystemBase {

  public:

   SystemBase(const std::string& name) : SystemBase(name, {}, {}) {}

   SystemBase(const std::string& name,
              std::initializer_list<uint32_t> dependencies,
              std::initializer_list<uint32_t> managed) : 
      m_enabled(false),
      m_name(name),
      m_dependencies(dependencies),
      m_managed(managed) {}

   virtual ~SystemBase() {}

   const bool enabled() const;

   void disable();
   void enable();

   virtual void update() = 0;
   virtual void render(float interpolation) = 0;
   virtual bool initialize() = 0;
   virtual void component_added(const uint32_t componentType, Entity e) {}
   
   const std::string& name() {
      return m_name;
   }

  protected:

   static EntityManager* s_entities;
   static Engine* s_engine;
   static EventDispatcher* s_dispatcher;

   bool m_enabled;

   std::string m_name;

  private:

   std::vector<uint32_t> m_dependencies;
   std::vector<uint32_t> m_managed;

   uint32_t m_id;

   template <class SystemType>
   friend class SystemId;

   friend class Engine;

   static uint32_t& counter() {
      static uint32_t counter = 0;
      return counter;
   }

};

inline const bool SystemBase::enabled() const { return m_enabled; }

inline void SystemBase::enable() { m_enabled = true; }
inline void SystemBase::disable() { m_enabled = false; }

template <class SystemType>
class SystemId {
  public:

   static uint32_t id() {
      static uint32_t id = SystemBase::counter()++;
      return id;
   }

};

}

#endif
