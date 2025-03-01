#ifndef CAMERA_HXX
#define CAMERA_HXX

#include <glm/glm.hpp>

namespace my {

struct Dimensions;

struct Camera {
public:
  Camera(float fovy, float aspect, float zNear, float zFar);
  Camera() = delete;
  Camera(const Camera&) = delete;
  Camera(Camera&&) = delete;
  Camera& operator=(const Camera&) = delete;
  Camera& operator=(Camera&&) = delete;
  auto getProjectionMatrix() const -> const glm::mat4&;
  auto getViewMatrix() const -> const glm::mat4&;
  auto getPosition() const -> const glm::vec3&;
  auto moveRight(float dx) -> void;
  auto moveUp(float dy) -> void;
  auto moveForward(float dz) -> void;
  auto pitch(float dax) -> void;
  auto yaw(float day) -> void;
  auto roll(float daz) -> void;
  auto resize(const Dimensions& size) -> void;
  auto updateView() -> void;

private:
  glm::mat4 _projection{1.};
  glm::mat4 _view{1.};
  glm::vec3 _position{0., 0., 0.};
  glm::vec3 _right{1., 0., 0.};
  glm::vec3 _up{0., 1., 0.};
  glm::vec3 _look{0., 0., -1.};
  float _fovy;
  float _aspect;
  float _zNear;
  float _zFar;

  auto updateProjection() -> void;
};

} // namespace my

#endif // CAMERA_HXX
