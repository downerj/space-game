#include <cstddef>
#include <exception>

#include "debug.hxx"
#include "game.hxx"
#include "graphics.hxx"
#include "model.hxx"
#include "window.hxx"

namespace {

Model cube{
  // vertices
  {
    // Front bottom left
    -1., -1., -1.,
    // Front bottom right
    1., -1., -1.,
    // Front top right
    1., 1., -1.,
    // Front top left
    -1., 1., -1.,
    // Back bottom left
    -1., -1., 1.,
    // Back bottom right
    1., -1., 1.,
    // Back top right
    1., 1., 1.,
    // Back top left
    -1., 1., 1.
  },
  // vertex colors
  {
    // Front bottom left
    1., 0., 0.,
    // Front bottom right
    1., 1., 0.,
    // Front top right
    0., 1., 0.,
    // Front top left
    0., 1., 1.,
    // Back bottom left
    0., 0., 1.,
    // Back bottom right
    1., 0., 1.,
    // Back top right
    0., 0., 0.,
    // Back top left
    1., 1., 1.
  },
  // indices
  {
    // Front
    0, 1, 2,
    0, 2, 3,
    // Right
    1, 5, 6,
    1, 6, 2,
    // Top
    3, 2, 6,
    3, 6, 7,
    // Back
    5, 4, 7,
    5, 7, 6,
    // Left
    4, 0, 3,
    4, 3, 7,
    // Bottom
    4, 5, 1,
    4, 1, 0
  }
};

} // namespace

auto main(int, char**) -> int {
  try {
    my::Window window{};
    my::Graphics graphics{};
    my::Game game{};
    graphics.setCamera(&game.getCamera());
    graphics.addModel(cube);

    while (window.update()) {
      game.update();
      game.resize(window.getSize());
      graphics.resize(window.getSize());
      graphics.render();
      window.postRender();
    }
  } catch (std::exception& ex) {
    std::cerr << ex.what() << '\n';
    std::exit(EXIT_FAILURE);
  }
  LOG("Goodbye.\n");
}
