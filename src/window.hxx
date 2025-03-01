#ifndef WINDOW_HXX
#define WINDOW_HXX

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

namespace my {

class Window {
public:
  Window();
  ~Window();
  auto update() -> bool;
  auto postRender() -> void;
  auto getHandle() const -> const SDL_Window*;
  auto getHandle() -> SDL_Window*;

private:
  SDL_Window* _window{nullptr};
  SDL_Event _event{};
};

} // namespace my

#endif // WINDOW_HXX
