#include "window.hxx"

#include <stdexcept>

#include <SDL2/SDL.h>

#include "debug.hxx"

namespace {

const char* title{"Space Game"};
const int initialWidth{400};
const int initialHeight{400};
const int glMajor{3};
const int glMinor{3};

auto initSDL() -> void;
auto createSDLWindow() -> SDL_Window*;
auto createGLContext(SDL_Window* window) -> SDL_GLContext;
auto setVSync() -> void;

} // namespace

my::Window::Window() {
  LOG("Initializing window\n");
  initSDL();
  _window = createSDLWindow();
  _context = createGLContext(_window);
  setVSync();
  updateSize();
  SDL_StartTextInput();
}

my::Window::~Window() {
  LOG("Cleaning up window\n");
  SDL_StopTextInput();
  SDL_DestroyWindow(_window);
  SDL_Quit();
  _window = nullptr;
}

auto my::Window::update() -> bool {
  while (SDL_PollEvent(&_event) != 0) {
    switch (_event.type) {
      case SDL_QUIT:
        return false;
      case SDL_WINDOWEVENT:
        if (_event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED
            || _event.window.event == SDL_WINDOWEVENT_RESIZED) {
          LOG("Window was resized!\n");
          updateSize();
        }
        break;
    }
  }
  return true;
}

auto my::Window::postRender() -> void {
  SDL_GL_SwapWindow(_window);
}

auto my::Window::getHandle() const -> const SDL_Window* {
  return _window;
}

auto my::Window::getHandle() -> SDL_Window* {
  return _window;
}

auto my::Window::getSize() const -> const Dimensions& {
  return _size;
}

auto my::Window::getSize() -> Dimensions& {
  return _size;
}

auto my::Window::updateSize() -> void {
  SDL_GetWindowSize(_window, &_size.width, &_size.height);
}

namespace {

auto initSDL() -> void {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG_ERROR("Failed to load SDL\n");
    throw std::runtime_error{"Failed to initialize windowing library"};
  }
}

auto createSDLWindow() -> SDL_Window* {
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajor);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinor);
  SDL_GL_SetAttribute(
    SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE
  );
  SDL_Window* window{SDL_CreateWindow(
    title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    initialWidth, initialHeight,
    SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  )};
  if (!window) {
    LOG_ERROR("Failed to create SDL window\n");
    throw std::runtime_error{"Failed to create window"};
  }
  return window;
}

auto createGLContext(SDL_Window* window) -> SDL_GLContext {
  SDL_GLContext context{SDL_GL_CreateContext(window)};
  if (!context) {
    LOG_ERROR("Failed to initialize SDL OpenGL context\n");
    throw std::runtime_error{"Failed to initialize graphics context"};
  }
  return context;
}

auto setVSync() -> void {
  if (SDL_GL_SetSwapInterval(1) < 0) {
    LOG_ERROR("Warning: Unable to set VSync: " << SDL_GetError() << '\n');
  }
}

} // namespace
