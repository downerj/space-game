#include "graphics.hxx"

#include <stdexcept>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <SDL2/SDL_error.h>

#include "debug.hxx"
#include "window.hxx"

namespace {

#ifdef DEBUG
auto debugMessageCallbackGL(
  GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
  const GLchar* message, const void* userParam
) -> void;
#endif // DEBUG

} // namespace

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
#ifdef DEBUG
  if (GLAD_GL_ARB_debug_output) {
    LOG("GL_ARB_debug_output extension available\n");
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
    glDebugMessageCallbackARB(
      debugMessageCallbackGL,
      nullptr // userParam
    );
  } else {
    LOG("GL_ARB_debug_output extension unavailable\n");
  }
#endif // DEBUG
}

my::Graphics::~Graphics() {
  gladLoaderUnloadGL();
}

auto my::Graphics::resize(const my::Dimensions& dimensions) -> void {
  glViewport(0, 0, dimensions.width, dimensions.height);
}

auto my::Graphics::render() const -> void {
  glClearColor(0., .5, 1., 1.);
  glClear(GL_COLOR_BUFFER_BIT);
}

namespace {

#ifdef DEBUG
auto debugMessageCallbackGL(
  GLenum, GLenum, GLuint, GLenum severity, GLsizei, const GLchar* message,
  const void*
) -> void {
  switch (severity) {
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
    case GL_DEBUG_SEVERITY_HIGH_ARB:
      LOG_ERROR("OpenGL error: " << message);
      break;
  }
}
#endif // DEBUG

} // namespace
