#ifndef WINDOW_HXX
#define WINDOW_HXX

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>

namespace my {

struct Dimensions {
  int width;
  int height;
};

class Window {
public:
  Window();
  Window(const Window&) = delete;
  Window(Window&&) = delete;
  Window& operator=(const Window&) = delete;
  Window& operator=(Window&&) = delete;
  ~Window();
  auto update() -> bool;
  auto postRender() -> void;
  auto getHandle() const -> const SDL_Window*;
  auto getHandle() -> SDL_Window*;
  auto getSize() const -> const Dimensions&;
  auto getSize() -> Dimensions&;

private:
  SDL_Window* _window{nullptr};
  SDL_Event _event{};
  Dimensions _size{};

  auto updateSize() -> void;
};

} // namespace my

#endif // WINDOW_HXX
