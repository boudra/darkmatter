#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

/*
#include "ObjectFactory.hpp"
#include "Manager.hpp"
#include "ComponentPtr.hpp"

#include <memory>

namespace dm {


class ComponentManager : public Manager {

  public:
   typedef std::map<ComponentType, ObjectFactory<Component>*>
       ComponentFactories;
   typedef std::map<std::string, ComponentType> ComponentTypes;

   template <class ValueType>
   std::vector<ValueType>& GetComponents() {
      const ComponentType& type = GetComponentType(ValueType);
      return m_factories[type]->GetVector();
   }

   template <class ValueType>
   ObjectFactory<ValueType>& GetComponentFactory() {

      const ComponentType& type = GetComponentType(ValueType);
      ComponentFactories::iterator found = m_factories.find(type);

      if (found == m_factories.end()) {

         auto pair = std::make_pair(type,
                                    reinterpret_cast<ObjectFactory<Component>*>(
                                        new ObjectFactory<ValueType>()));

         found = m_factories.insert(pair).first;

         ComponentPtr<ValueType>::m_componentFactory =
             reinterpret_cast<ObjectFactory<ValueType>*>(found->second);
      }

      return *reinterpret_cast<ObjectFactory<ValueType>*>(found->second);
   }

   template <class ValueType>
   void Register(const std::string& name) {
      m_types.insert(std::make_pair(name, GetComponentType(ValueType)));
   }

   template <class ValueType>
   const ComponentPtr<ValueType> Create() {
      return this->GetComponentFactory<ValueType>().Create();
   }

   template <class ReturnType = Component>
   const ComponentPtr<ReturnType> Create(const std::string& componentName) {
      ComponentTypes::iterator found = m_types.find(componentName)->second;

      if (found == m_types.end()) {
         return this->GetComponentFactory<ReturnType>().Create();
      }

      return this->GetComponentFactory<ReturnType>(found->second).Create();
   }

  private:
   ComponentFactories m_factories;
   ComponentTypes m_types;
};
}
*/
#endif
