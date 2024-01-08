#ifndef ROAD_H
#define ROAD_H

#include "../../Configs.h"
#include <raylib-cpp.hpp>
#include <iostream>

namespace CW {

class Road
{
public:
  std::vector<Vector2> path;
  Color color;
  Fraction fraction;

  Road();
  Road(std::vector<Vector2> path, Color color, Fraction fraction);

  void Draw();
};

}  // namespace CW

#endif  // ROAD_H