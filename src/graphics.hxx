#ifndef GRAPHICS_HXX
#define GRAPHICS_HXX

#include <vector>

#include <glad/gl.h>
#include <SDL2/SDL_video.h>

#include "camera.hxx"
#include "model.hxx"

namespace my {

struct Dimensions;

struct Actor {
  Model& model;
  GLuint vao;
  GLuint positionBuffer;
  GLuint colorBuffer;
  GLuint indexBuffer;

  auto cleanUp() const -> void;
};

class Graphics {
public:
  Graphics();
  Graphics(const Graphics&) = delete;
  Graphics(Graphics&&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics& operator=(Graphics&&) = delete;
  ~Graphics();
  auto addModel(Model& model) -> void;
  auto setCamera(const Camera* camera) -> void;
  auto resize(const Dimensions& dimensions) -> void;
  auto render() const -> void;

private:
  int _glVersion;
  GLuint _mainProgram;
  std::vector<Actor> _actors{};
  const Camera* _camera{nullptr};
  GLint _projectionUniform;
  GLint _viewUniform;
  GLint _positionLocation;
  GLint _colorLocation;
};

} // namespace my

#endif // GRAPHICS_HXX
