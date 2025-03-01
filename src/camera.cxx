#include "camera.hxx"

#include <glm/gtc/matrix_transform.hpp>

#include "window.hxx"

my::Camera::Camera(float fovy, float aspect, float zNear, float zFar) :
_fovy{fovy}, _aspect{aspect}, _zNear{zNear}, _zFar{zFar} {
  updateProjection();
}

auto my::Camera::getProjectionMatrix() const -> const glm::mat4& {
  return _projection;
}

auto my::Camera::getViewMatrix() const -> const glm::mat4& {
  return _view;
}

auto my::Camera::getPosition() const -> const glm::vec3& {
  return _position;
}

auto my::Camera::moveRight(float dx) -> void {
  _position -= _right * dx;
}

auto my::Camera::moveUp(float dy) -> void {
  _position -= _up * dy;
}

auto my::Camera::moveForward(float dz) -> void {
  _position += _look * dz;
}

auto my::Camera::pitch(float dax) -> void {
  
}

auto my::Camera::yaw(float day) -> void {

}

auto my::Camera::roll(float daz) -> void {

}

auto my::Camera::resize(const Dimensions& size) -> void {
  _aspect = static_cast<float>(size.width) / static_cast<float>(size.height);
  updateProjection();
}

auto my::Camera::updateView() -> void {
  _view = glm::mat4{1.};
  _view = glm::translate(_view, _position);
}

auto my::Camera::updateProjection() -> void {
  _projection = glm::perspective(glm::radians(_fovy), _aspect, _zNear, _zFar);
}
