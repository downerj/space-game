#ifndef MODEL_HXX
#define MODEL_HXX

#include <vector>

struct Color {
  int r;
  int g;
  int b;
  int a;
};

struct Material {
  Color color;
};

struct Model {
  std::vector<float> vertices;
  std::vector<float> vertexColors;
  std::vector<unsigned short> indices;
  // Material material;
};

#endif // MODEL_HXX
