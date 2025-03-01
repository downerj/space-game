#include <cstddef>
#include <exception>

#include "debug.hxx"
#include "game.hxx"
#include "graphics.hxx"
#include "window.hxx"

auto main(int, char**) -> int {
  try {
    my::Window window{};
    my::Graphics graphics{window.getHandle()};
    my::Game game{}; (void)game;

    while (window.update()) {
      graphics.render();
      window.postRender();
    }
  } catch (std::exception& ex) {
    std::cerr << ex.what() << '\n';
    std::exit(EXIT_FAILURE);
  }
  LOG("Goodbye.\n");
}
