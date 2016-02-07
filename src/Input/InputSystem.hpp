#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "Core/System.hpp"
#include "Event/EventHandler.hpp"
#include "Math/Vector2.hpp"

struct SDL_Window;

namespace dm {

class InputSystem : public System {
   public:
    InputSystem() {}
    ~InputSystem() {}

    void render(const GameState&);
    void update(GameState&);

    void set_window(SDL_Window* window);

   private:
    SDL_Window* m_window;
    Vec2i m_window_size;
    Vec2f m_mouse_transform;
};
dm_internal_register_type(InputSystem)
}

#endif
