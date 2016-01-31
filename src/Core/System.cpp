#include "System.hpp"

namespace dm {
EntityManager *SystemBase::s_entities = nullptr;
Engine *SystemBase::s_engine = nullptr;
EventDispatcher *SystemBase::s_dispatcher = nullptr;
}
