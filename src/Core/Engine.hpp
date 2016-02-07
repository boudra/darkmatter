#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "System.hpp"
#include "Logger.hpp"
#include "Manager.hpp"

#include "Event/EventDispatcher.hpp"
#include "Core/GameState.hpp"

#include <vector>
#include <unordered_map>

namespace dm {

class Window;

class Engine {
   public:
    Engine();
    ~Engine();

    bool initialize();
    bool start();

    void update();
    void render();

    Window* window();

    void quit(const QuitEvent& e);

    template <class system_type>
    inline void add_system(system_type* system) {
        add_system(std::shared_ptr<system_type>(system));
    }

    template <class system_type>
    inline void add_system(std::shared_ptr<system_type> system) {
        system->enable();
        m_systems[dm_type_name(system_type)] = system;
        Log::info("System %s added", dm_type_name(system_type));
    }

    template <class system_type>
    inline std::shared_ptr<system_type> get_system(const std::string& name) {
        return std::static_pointer_cast<system_type>(
            m_systems[dm_type_name(system_type)]);
    }

    template <class system_type>
    inline std::shared_ptr<system_type> get_system() {
        return std::static_pointer_cast<system_type>(
            m_systems[dm_type_name(system_type)]);
    }

    template <class ManagerType>
    ManagerType* manager();

   private:
    template <class ManagerType>
    bool load_manager();

    EventDispatcher m_dispatcher;

    Window* m_window;
    GameState state_;

    bool m_exit;

    std::unordered_map<std::string, std::shared_ptr<System>> m_systems;
    std::vector<ManagerBase*> m_managers;
};

dm_internal_register_type(Engine);

inline Window* Engine::window() { return m_window; }

template <class ManagerType>
inline bool Engine::load_manager() {
    const ManagerId managerId = ManagerType::id();
    auto* manager = new ManagerType();

    if (m_managers.size() <= managerId) {
        m_managers.resize(managerId + 1);
    }

    m_managers[managerId] = static_cast<ManagerBase*>(manager);
    manager->initialize(state_);

    return true;
}
template <class ManagerType>
inline ManagerType* Engine::manager() {
    assert(m_managers.size() > ManagerType::id(), "Manager not found");
    return static_cast<ManagerType*>(m_managers[ManagerType::id()]);
}

#define GET_MANAGER(TYPE) s_engine->manager<TYPE>()
}

#endif
