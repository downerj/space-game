#include <iostream>

#include "game.hxx"

auto main(int, char**) -> int {
  Game game{}; (void)game;
  std::cout << "Game initialized!\n";
}
