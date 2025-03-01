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

} // namespace

my::Window::Window() {
  LOG("Initializing window\n");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG_ERROR("Failed to load SDL");
    throw std::runtime_error{"Failed to initialize windowing library"};
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajor);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinor);
  SDL_GL_SetAttribute(
    SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE
  );
  _window = SDL_CreateWindow(
    title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
    initialWidth, initialHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
  );
  if (!_window) {
    LOG_ERROR("Failed to create SDL window\n");
    throw std::runtime_error{"Failed to create window"};
  }
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
    if (_event.type == SDL_QUIT) {
      return false;
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
