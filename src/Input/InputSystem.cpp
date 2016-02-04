#include "InputSystem.hpp"

#include <SDL2/SDL.h>

#include "Core/Logger.hpp"
#include "System/Window.hpp"

namespace dm {

void InputSystem::set_window(SDL_Window* window) {
    m_window = window;

    SDL_GetWindowSize(window, &m_window_size.x, &m_window_size.y);

    m_mouse_transform.x = static_cast<float>(VIEWPORT_WIDTH) /
                          static_cast<float>(m_window_size.x);

    m_mouse_transform.y = static_cast<float>(VIEWPORT_HEIGHT) /
                          static_cast<float>(m_window_size.y);
}

void InputSystem::update() {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_QUIT:
                s_dispatcher->send_event(QuitEvent());
                break;

            case SDL_KEYDOWN:
                if (sdl_event.key.repeat != 0) continue;
                s_dispatcher->send_event(
                    KeyboardEvent(sdl_event.key.keysym.sym, true));
                break;

            case SDL_KEYUP:
                s_dispatcher->send_event(
                    KeyboardEvent(sdl_event.key.keysym.sym, false));
                break;

            case SDL_MOUSEMOTION: {
                Vec2i mouse_position;
                Vec2f mouse_transformed;
                SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
                mouse_transformed.x = mouse_position.x * m_mouse_transform.x;
                mouse_transformed.y =
                    (m_window_size.y - mouse_position.y) * m_mouse_transform.y;
                s_dispatcher->send_event(MouseEvent(mouse_transformed));
                break;
            }

            case SDL_WINDOWEVENT: {
                switch (sdl_event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        Log::info("Window resized");
                        break;

                    default:
                        break;
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                Vec2i mouse_position;
                Vec2f mouse_transformed;
                SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
                mouse_transformed.x = mouse_position.x * m_mouse_transform.x;
                mouse_transformed.y =
                    (m_window_size.y - mouse_position.y) * m_mouse_transform.y;
                s_dispatcher->send_event(
                    MouseEvent(mouse_transformed, "mouse_click"));
                break;
            }

            case SDL_MOUSEWHEEL: {
                Vec2i delta{sdl_event.wheel.x, sdl_event.wheel.y};
                s_dispatcher->send_event(MouseWheelEvent(delta));
                break;
            }

            default:
                break;
        }
    }
}

void InputSystem::render(float interpolation) {}

bool InputSystem::initialize() { return true; }
}
