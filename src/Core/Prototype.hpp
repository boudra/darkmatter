#ifndef PROTOTYPE_HPP
#define PROTOTYPE_HPP

#include <map>
#include <string>

#include "TypeId/Any.hpp"

namespace dm {

class Prototype {
  public:
   Prototype();
   ~Prototype();

   template <class ObjectType>
   ObjectType Create(std::string name) {
      return m_prototypes[name].Cast<ObjectType>();
   }

  private:
   std::map<std::string, Any> m_prototypes;
};

template <>
Any Prototype::Create(std::string name) {
   return m_prototypes[name];
}

} /* namespace dm */

#endif /* end of include guard: PROTOTYPE_HPP */
