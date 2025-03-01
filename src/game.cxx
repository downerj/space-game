#include "game.hxx"

#include "window.hxx"

my::Game::Game() {
  _camera.moveForward(-5);
}

auto my::Game::getCamera() const -> const my::Camera& {
  return _camera;
}

auto my::Game::resize(const Dimensions& size) -> void {
  _camera.resize(size);
}

auto my::Game::update() -> void {
  _camera.updateView();
}
