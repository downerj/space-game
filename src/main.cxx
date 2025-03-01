#include <iostream>

#include "game.hxx"
#include "graphics.hxx"
#include "window.hxx"

auto main(int, char**) -> int {
  my::Window window{}; (void)window;
  std::cout << "Window initialized!\n";
  my::Graphics graphics{}; (void)graphics;
  std::cout << "Graphics initialized!\n";
  my::Game game{}; (void)game;
  std::cout << "Game initialized!\n";
  
  std::cout << "Goodbye.\n";
}
