#ifndef GRAPHICS_HXX
#define GRAPHICS_HXX

#include <SDL2/SDL_video.h>

namespace my {

struct Dimensions;

class Graphics {
public:
  Graphics(SDL_Window* window);
  Graphics() = delete;
  Graphics(const Graphics&) = delete;
  Graphics(Graphics&&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics& operator=(Graphics&&) = delete;
  ~Graphics();
  auto resize(const Dimensions& dimensions) -> void;
  auto render() const -> void;

private:
  SDL_GLContext _context;
};

} // namespace my

#endif // GRAPHICS_HXX
