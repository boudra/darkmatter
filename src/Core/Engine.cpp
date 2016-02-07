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
    for (auto& vk : m_systems) {
        state_.unsubscribe(vk.second.get());
    }
    for (auto& manager : m_managers) {
        state_.unsubscribe(manager);
        delete manager;
    }
}

bool Engine::initialize() {
    Log::info("Starting engine");

    m_window = new Window();

    if (!m_window->create()) return false;
    if (!m_window->create_context()) return false;

    this->load_manager<ResourceManager>();

    state_.set("resource_manager", *this->manager<ResourceManager>());

    this->add_system(new InputSystem());
    this->add_system(new Camera());
    this->add_system(new RenderSystem());

    this->get_system<InputSystem>()->set_window(m_window->handle());

    Log::ok("Engine started");

    state_.subscribe("quit", this, &Engine::quit);
    state_.transition_to("initial");

    return true;
}

void Engine::quit(const QuitEvent& e) {
    Log::info("Quit");
    m_exit = true;
}

void Engine::update() {
    for (auto& s : m_systems) {
        if (s.second->enabled()) s.second->update(this->state_);
    }
}

void Engine::render() {
    m_window->clear();

    for (auto& s : m_systems) {
        if (s.second->enabled()) s.second->render(this->state_);
    }

    m_window->swap_buffers();
}

bool Engine::start() {
    using milliseconds =
        std::chrono::duration<double, std::chrono::milliseconds::period>;

    const auto tick_time = milliseconds(16.0);
    auto last = std::chrono::high_resolution_clock::now();
    decltype(last) current;
    milliseconds acumulator, elapsed;
    bool skipFrame = false;

    while (!m_exit) {
        skipFrame = true;
        current = std::chrono::high_resolution_clock::now();
        elapsed = milliseconds(current - last);
        acumulator += elapsed;
        last = current;

        // Log::debug("%lf", elapsed.count());

        while (acumulator >= tick_time) {
            skipFrame = false;
            this->update();
            acumulator -= tick_time;
            state_.transition_to("pou");
        }

        if (!skipFrame) {
            this->state_.set("frame_time", elapsed.count());
            this->render();
        }
    }

    return true;
}

} /* namespace dm */
