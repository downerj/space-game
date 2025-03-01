#ifndef GAME_HXX
#define GAME_HXX

#include "camera.hxx"
#include "player.hxx"

namespace my {

class Game {
public:

private:
  Camera _camera{};
  Player _player{};
};

} // namespace my

#endif // GAME_HXX
