#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "System.hpp"
#include "Logger.hpp"
#include "Manager.hpp"

#include "Event/EventDispatcher.hpp"

#include <vector>

namespace dm {

class Window;

class Engine {
   public:
    Engine();
    ~Engine();

    bool initialize();
    bool start();

    void update();
    void render(float interpolation);

    Window *window();

    void quit(const QuitEvent &e);

    template <class SystemType>
    SystemType *system();

    template <class ManagerType>
    ManagerType *manager();

   private:
    template <class SystemType>
    uint32_t add_system();

    template <class ManagerType>
    bool load_manager();

    EventDispatcher m_dispatcher;

    Window *m_window;

    bool m_exit;

    std::vector<SystemBase *> m_systems;
    std::vector<ManagerBase *> m_managers;
};

inline Window *Engine::window() { return m_window; }

template <class SystemType>
inline uint32_t Engine::add_system() {
    const uint32_t systemId = SYSTEM(SystemType);
    auto *system = new SystemType();

    if (m_systems.size() <= systemId) {
        m_systems.resize(systemId + 1);
    }

    system->m_id = systemId;

    m_systems[systemId] = static_cast<SystemBase *>(system);

    return systemId;
}

template <class ManagerType>
inline bool Engine::load_manager() {
    const ManagerId managerId = ManagerType::id();
    auto *manager = new ManagerType();

    if (m_managers.size() <= managerId) {
        m_managers.resize(managerId + 1);
    }

    m_managers[managerId] = static_cast<ManagerBase *>(manager);
    manager->initialize();

    return true;
}
template <class SystemType>
inline SystemType *Engine::system() {
    assert(m_systems.size() > SYSTEM(SystemType));
    return static_cast<SystemType *>(m_systems[SYSTEM(SystemType)]);
}

template <class ManagerType>
inline ManagerType *Engine::manager() {
    assert(m_managers.size() > ManagerType::id());
    return static_cast<ManagerType *>(m_managers[ManagerType::id()]);
}

#define GET_SYSTEM(TYPE) s_engine->system<TYPE>()

#define GET_MANAGER(TYPE) s_engine->manager<TYPE>()
}

#endif
