#include "Window.hpp"
#include "Core/Logger.hpp"

#include <iostream>
#include <SDL2/SDL.h>

#include <GL/glew.h>

#ifdef WIN32
#include <GL/wglew.h>
#endif

namespace dm {

Window::Window()
    : m_caption(NULL),
      m_size(0, 0),
      m_position(0, 0),
      m_created(false),
      m_hascontext(false) {}

Window::~Window() {}

SDL_Window *Window::handle() { return sdl_window; }

bool Window::create() {
   return this->create(DEFAULT_CAPTION, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

bool Window::create(const char *caption) {
   return this->create(caption, DEFAULT_WIDTH, DEFAULT_HEIGHT);
}

/*
Create the window using SDL2
*/
bool Window::create(const char *caption, int width, int height) {

   SDL_Init(SDL_INIT_VIDEO);
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
   SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
   SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
   SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
   SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                       SDL_GL_CONTEXT_PROFILE_CORE);

   sdl_window = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, width, height,
                                 SDL_WINDOW_OPENGL);

   Log::progress("debug", "Creating window");

   if (!sdl_window) {
     Log::result(Log::Result::ERROR);
     Log::error("Window creation failed: %s", SDL_GetError());
      SDL_Quit();
      return false;
   }

   Log::result(Log::Result::OK);

   m_created = true;
   m_size = Vec2i(width, height);
   m_caption = new char[strlen(caption)];

#ifdef _WIN32
   strcpy_s(m_caption, strlen(caption) + 1, caption);
#else
   strcpy(m_caption, caption);
#endif

   return true;
}

/*
Create an OpenGL context
*/
bool Window::create_context() {

   Log::progress("debug", "Creating context");

   if (!m_created && !m_hascontext) {
     Log::result(Log::Result::ERROR);
     Log::error("Trying to create a context without a window!");
      return false;
   }

   sdl_context = SDL_GL_CreateContext(sdl_window);

   if (!sdl_context) {
     Log::result(Log::Result::ERROR);
     Log::error("OpenGL context creation failed: %s", SDL_GetError());
      return false;
   }

   Log::result(Log::Result::OK);

   m_hascontext = true;

//   glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
   glClearColor(0.f, 0.f, 0.f, 1.0f);

   int msaa = 0;

   SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &msaa);

   // LOG(DEBUG, "GL_VERSION " << glGetString(GL_VERSION));
   // LOG(DEBUG, "GL_VENDOR " << glGetString(GL_VENDOR));
   // LOG(DEBUG, "GL_RENDERER " << glGetString(GL_RENDERER));
   // LOG(DEBUG, "MSAA " << msaa);

   // VSync
   SDL_GL_SetSwapInterval(1);

   glewExperimental = GL_TRUE; 
   glewInit();

   glEnable(GL_MULTISAMPLE);

   glViewport(0, 0, m_size.x, m_size.y);

   return true;
}

void Window::swap_buffers() { SDL_GL_SwapWindow(sdl_window); }
void Window::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

} /* game */
