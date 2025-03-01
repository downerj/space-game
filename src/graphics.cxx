#include "graphics.hxx"

#include <stdexcept>
#include <string>
#include <string_view>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_error.h>

#include "debug.hxx"
#include "window.hxx"

namespace {

#ifdef DEBUG
auto debugMessageCallbackGL(
  GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
  const GLchar* message, const void* userParam
) -> void;
auto setGLDebug() -> void;
auto logGLInfo(int glVersion) -> void;
#endif // DEBUG
auto loadGL() -> int;
auto createShader(GLenum type, std::string_view source) -> GLuint;
auto createProgram(
  std::string_view vertexSource, std::string_view fragmentSource
) -> GLuint;
template<typename T>
auto createBuffer(
  GLenum target, const T& data, GLenum usage = GL_STATIC_DRAW
) -> GLuint;
auto getUniformLocation(GLuint program, std::string_view name) -> GLint;
auto getAttributeLocation(GLuint program, std::string_view name) -> GLint;

#include "shaders/main.vert"
#include "shaders/main.frag"

} // namespace

auto my::Actor::cleanUp() const -> void {
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &positionBuffer);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteBuffers(1, &indexBuffer);
}

my::Graphics::Graphics() :
_glVersion{loadGL()},
_mainProgram{createProgram(mainVertexSource, mainFragmentSource)},
_projectionUniform{getUniformLocation(_mainProgram, "projection")},
_viewUniform{getUniformLocation(_mainProgram, "view")},
_positionLocation{getAttributeLocation(_mainProgram, "position")},
_colorLocation{getAttributeLocation(_mainProgram, "color")} {}

my::Graphics::~Graphics() {
  glDeleteProgram(_mainProgram);
  for (const auto& actor : _actors) {
    actor.cleanUp();
  }
  gladLoaderUnloadGL();
}

auto my::Graphics::addModel(Model& model) -> void {
  const GLuint positionBuffer{createBuffer(GL_ARRAY_BUFFER, model.vertices)};
  const GLuint colorBuffer{createBuffer(GL_ARRAY_BUFFER, model.vertexColors)};
  const GLuint indexBuffer{
    createBuffer(GL_ELEMENT_ARRAY_BUFFER, model.indices)
  };

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
  glVertexAttribPointer(
    _positionLocation,
    static_cast<GLint>(sizeof(float)*model.vertices.size()),
    GL_FLOAT,
    false, // normalized
    0, // stride
    nullptr // pointer
  );
  glEnableVertexAttribArray(_positionLocation);

  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(
    _colorLocation,
    static_cast<GLint>(sizeof(float)*model.vertexColors.size()),
    GL_FLOAT,
    false, // normalized,
    0, // stride
    nullptr // pointer
  );
  glEnableVertexAttribArray(_colorLocation);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  _actors.push_back({model, vao, positionBuffer, colorBuffer, indexBuffer});
}

auto my::Graphics::setCamera(const Camera* camera) -> void {
  _camera = camera;
}

auto my::Graphics::resize(const my::Dimensions& dimensions) -> void {
  glViewport(0, 0, dimensions.width, dimensions.height);
}

auto my::Graphics::render() const -> void {
  glClearColor(0., .5, 1., 1.);
  glClear(GL_COLOR_BUFFER_BIT);
  if (!_camera) {
    return;
  }
  glUseProgram(_mainProgram);
  const glm::mat4& projection{_camera->getProjectionMatrix()};
  const glm::mat4& view{_camera->getViewMatrix()};
  glUniformMatrix4fv(_projectionUniform, 1, false, glm::value_ptr(projection));
  glUniformMatrix4fv(_viewUniform, 1, false, glm::value_ptr(view));
  for (const auto& actor : _actors) {
    glBindVertexArray(actor.vao);
    glDrawElements(
      GL_TRIANGLES, static_cast<GLsizei>(actor.model.indices.size()),
      GL_UNSIGNED_SHORT,
      nullptr // indices
    );
  }
  glBindVertexArray(0);
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

auto logGLInfo(int glVersion) -> void {
  LOG("Initialized OpenGL " << GLAD_VERSION_MAJOR(glVersion)
    << '.' << GLAD_VERSION_MINOR(glVersion) << '\n');
  LOG("Driver OpenGL version: " << glGetString(GL_VERSION) << '\n');
}

auto setGLDebug() -> void {
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
}
#endif // DEBUG

auto loadGL() -> int {
  const int version{gladLoaderLoadGL()};
  if (!version) {
    LOG_ERROR("Failed to load OpenGL\n");
    throw std::runtime_error{"Failed to intialize OpenGL"};
  }
#ifdef DEBUG
  logGLInfo(version);
  setGLDebug();
#endif // DEBUG
  return version;
}

auto createShader(GLenum type, std::string_view source) -> GLuint {
  const GLuint shader{glCreateShader(type)};
  const GLchar* const sources[]{source.data()};
  const GLint lengths[]{static_cast<GLint>(source.size())};
  glShaderSource(shader, 1, sources, lengths);
  glCompileShader(shader);
  return shader;
}

auto createProgram(
  std::string_view vertexSource, std::string_view fragmentSource
) -> GLuint {
  const GLuint vertexShader{
    createShader(GL_VERTEX_SHADER, vertexSource)
  };
  const GLuint fragmentShader{
    createShader(GL_FRAGMENT_SHADER, fragmentSource)
  };
  const GLuint program{glCreateProgram()};
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  GLint status{};
  glGetProgramiv(program, GL_LINK_STATUS, &status);
#ifdef DEBUG
  if (!status) {
    std::string log{};
    GLint logLength{};

    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    log.resize(logLength);
    glGetProgramInfoLog(program, logLength, nullptr /*length*/, log.data());
    LOG_ERROR("OpenGL shader program error: " << log << '\n');
    log.clear();
    
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      log.resize(logLength);
      glGetShaderInfoLog(
        vertexShader, logLength, nullptr /*length*/, log.data()
      );
      LOG_ERROR("OpenGL vertex shader error: " << log << '\n');
      log.clear();
    }

    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
      log.resize(logLength);
      glGetShaderInfoLog(
        fragmentShader, logLength, nullptr /*length*/, log.data()
      );
      LOG_ERROR("OpenGL fragment shader error: " << log << '\n');
      log.clear();
    }
  }
#endif // DEBUG

  glDetachShader(program, vertexShader);
  glDeleteShader(vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteShader(fragmentShader);

  if (!status) {
    glDeleteProgram(program);
    throw std::runtime_error{"Failed to create shader program"};
  }

  return program;
}

template<typename T>
auto createBuffer(GLenum target, const T& data, GLenum usage) -> GLuint {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(target, buffer);
  glBufferData(target, sizeof(T)*data.size(), data.data(), usage);
  glBindBuffer(target, 0);
  return buffer;
}

auto getUniformLocation(GLuint program, std::string_view name) -> GLint {
  GLint location{glGetUniformLocation(program, name.data())};
  if (location < 0) {
    LOG_ERROR("Attempt to get location for invalid uniform " << name << '\n');
    throw std::runtime_error{"Error accessing OpenGL shader uniform"};
  }
  return location;
}

auto getAttributeLocation(GLuint program, std::string_view name) -> GLint {
  GLint location{glGetAttribLocation(program, name.data())};
  if (location < 0) {
    LOG_ERROR(
      "Attempt to get location for invalid attribute " << name << '\n'
    );
    throw std::runtime_error{"Error accessing OpenGL shader attribute"};
  }
  return location;
}

} // namespace
