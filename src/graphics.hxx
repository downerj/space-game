#ifndef GRAPHICS_HXX
#define GRAPHICS_HXX

#include <SDL2/SDL_video.h>

namespace my {

class Graphics {
public:
  Graphics(SDL_Window* window);
  Graphics() = delete;
  ~Graphics();
  auto render() const -> void;

private:
  SDL_GLContext _context;
};

} // namespace my

#endif // GRAPHICS_HXX
