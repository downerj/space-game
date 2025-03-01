#ifndef GAME_HXX
#define GAME_HXX

#include "camera.hxx"
#include "player.hxx"

namespace my {

struct Dimensions;

class Game {
public:
  Game();
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;
  auto resize(const Dimensions& size) -> void;
  auto getCamera() const -> const Camera&;
  auto update() -> void;

private:
  Camera _camera{60.f, 1.f, .1f, 1000.f};
  Player _player{};
};

} // namespace my

#endif // GAME_HXX
