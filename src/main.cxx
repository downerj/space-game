#include <iostream>

#include "game.hxx"
#include "graphics.hxx"
#include "window.hxx"

auto main(int, char**) -> int {
  my::Window window{}; (void)window;
  my::Graphics graphics{}; (void)graphics;
  my::Game game{}; (void)game;
  
  std::cout << "Goodbye.\n";
}
