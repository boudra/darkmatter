#ifndef m_WINDOW_HPP_
#define m_WINDOW_HPP_

#include "Math/Vector2.hpp"

struct SDL_Window;

namespace dm {

const char DEFAULT_CAPTION[] = "Game";

const int DEFAULT_WIDTH = 1280;
const int DEFAULT_HEIGHT = 720;

const int VIEWPORT_WIDTH = 1280;
const int VIEWPORT_HEIGHT = 720;

class Window {
   public:
    Window();
    ~Window();

    bool create();
    bool create(const char *caption);
    bool create(const char *caption, int width, int height);

    bool create_context();

    const Vec2i &size() const;
    const Vec2i &position() const;
    const int width() const;
    const int height() const;

    void swap_buffers();
    void clear();

    SDL_Window *handle();

   private:
    char *m_caption;

    Vec2i m_size;
    Vec2i m_position;

    bool m_created;
    bool m_hascontext;

    SDL_Window *sdl_window;
    void *sdl_context;
};

inline const Vec2i &Window::size() const { return m_size; }
inline const Vec2i &Window::position() const { return m_position; }
inline const int Window::width() const { return m_size.x; }
inline const int Window::height() const { return m_size.y; }

} /* game */

#endif /* end of include guard: m_WINDOW_HPP_ */
