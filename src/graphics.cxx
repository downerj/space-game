#include "graphics.hxx"

#include <stdexcept>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <SDL2/SDL_error.h>

#include "debug.hxx"

my::Graphics::Graphics(SDL_Window* window)
: _context{SDL_GL_CreateContext(window)} {
  if (!_context) {
    LOG_ERROR("Failed to initialize SDL OpenGL context\n");
    throw std::runtime_error{"Failed to initialize graphics context"};
  }
  const int glVersion{gladLoaderLoadGL()};
  if (!glVersion) {
    LOG_ERROR("Failed to load OpenGL");
    throw std::runtime_error{"Failed to intialize graphics library"};
  }
  LOG(
    "Initialized OpenGL " << GLAD_VERSION_MAJOR(glVersion)
      << '.' << GLAD_VERSION_MINOR(glVersion) << '\n'
  );
  if (SDL_GL_SetSwapInterval(1) < 0) {
    LOG_ERROR("Warning: Unable to set VSync: " << SDL_GetError() << '\n');
  }
}

my::Graphics::~Graphics() {
  gladLoaderUnloadGL();
}

auto my::Graphics::render() const -> void {
  glClearColor(0., .5, 1., 1.);
  glClear(GL_COLOR_BUFFER_BIT);
}
