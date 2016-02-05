#include "Core/Engine.hpp"

#include "Core/EntityManager.hpp"
#include "Event/EventDispatcher.hpp"
#include "EntityManager.hpp"
#include "Input/InputSystem.hpp"
#include "Input/PlayerManager.hpp"
#include "Core/Logger.hpp"
#include "Core/ComponentManager.hpp"
#include "Animation/Animation.hpp"
#include "System/Window.hpp"
#include "Physics/PhysicsSystem.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/World.hpp"
#include "Graphics/RenderSystem.hpp"
#include "Resource/ResourceManager.hpp"
#include "Serialize/Serializer.hpp"

#include <SDL2/SDL.h>
#include <thread>
#include <future>

namespace dm {

Engine::Engine() : m_window(nullptr), m_exit(false) {}

Engine::~Engine() {
    m_dispatcher.unsubscribe(this);
    for(auto& system : m_systems) {
        delete system;
    }
    for(auto& manager : m_managers) {
        // manager->~BaseManager();
        delete manager;
    }
}

bool Engine::initialize() {
    Log::info("Starting engine");

    ManagerBase::s_engine = this;
    SystemBase::s_engine = this;

    ManagerBase::s_dispatcher = &m_dispatcher;
    SystemBase::s_dispatcher = &m_dispatcher;

    m_window = new Window();

    if (!m_window->create()) return false;
    if (!m_window->create_context()) return false;

    this->load_manager<PlayerManager>();
    this->load_manager<ResourceManager>();

    Deserializer::m_resource_manager = this->manager<ResourceManager>();

    // SystemBase::s_entities = this->manager<EntityManager>();
    // SystemBase::s_entities->create();

    this->add_system<RenderSystem>();
    this->add_system<InputSystem>();
    this->add_system<PhysicsSystem>();
    this->add_system<AnimationSystem>();
    this->add_system<Camera>();
    this->add_system<World>();

    std::vector<bool> initialized;
    initialized.resize(m_systems.size());

    uint32_t initializedCount = 0;

    auto initialize_system = [this, &initialized,
                              &initializedCount](SystemBase* system) {

        if (!system->initialize()) {
            Log::error("Failed to initialize %s", system->name().c_str());

            /* Cleanup */
            delete system;

            return false;
        }

        system->enable();
        initialized[system->m_id] = true;

        Log::ok("%s initialized", system->name().c_str());

        initializedCount++;

        // this->manager<EntityManager>()->set_managed_components(system->m_managed,
        // system);

        return true;

    };

    while (initializedCount < m_systems.size()) {
        for (SystemBase* system : m_systems) {
            if (initialized[system->m_id] == true) continue;

            if (system->m_dependencies.empty()) {
                initialize_system(system);
            } else {
                bool satisfied =
                    std::all_of(system->m_dependencies.begin(),
                                system->m_dependencies.end(),
                                [&initialized](const uint32_t& dep) {
                                    return initialized[dep];
                                });

                if (satisfied) {
                    initialize_system(system);
                }
            }
        }
    }

    this->system<InputSystem>()->set_window(m_window->handle());

    Log::ok("Engine started");

    m_dispatcher.subscribe("quit", this, &Engine::quit);

    return true;
}

void Engine::quit(const QuitEvent& e) {
    Log::info("Quit");
    m_exit = true;
}

void Engine::update() {
    std::future<void> futures[30];
    for (size_t i = 0; i < m_systems.size(); ++i) {
        SystemBase* s = m_systems[i];
        if (s->enabled()) {
            if (s->name() == "Input") {
                s->update();
            } else {
                futures[i] = std::async(std::launch::async, [s]() {
                    std::lock_guard<std::mutex> lk1(
                        MemoryPool<Entity>::mutex());
                    s->update();
                });
            }
        }
    }
    for (std::future<void>& f : futures) {
        if (f.valid()) {
            f.wait();
        }
    }
}

void Engine::render(float interpolation) {
    m_window->clear();

    for (SystemBase* s : m_systems) {
        s->render(interpolation);
    }

    m_window->swap_buffers();
}

bool Engine::start() {
    uint32_t last = 0, current = 0, acumulator = 0, elapsed = 0;
    bool skipFrame = false;

    while (!m_exit) {
        skipFrame = true;
        current = SDL_GetTicks();
        elapsed = current - last;
        acumulator += elapsed;
        last = current;

        while (acumulator >= TICK_TIME) {
            skipFrame = false;
            this->update();
            acumulator -= TICK_TIME;
        }

        if (!skipFrame) {
            this->system<RenderSystem>()->set_frame_time(elapsed);
            this->render(0.0f);
        }
    }

    return true;
}

} /* namespace dm */
