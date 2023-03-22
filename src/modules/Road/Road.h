#ifndef ROAD_H
#define ROAD_H

#include "../../Configs.h"
#include <raylib-cpp.hpp>
#include <iostream>

class Road
{
public:
  std::vector<Vector2> path;
  Color color;
  Fraction fraction;

  Road();
  Road(std::vector<Vector2> path, Color color, Fraction fraction);

  void Draw();

  static void DrawAll(std::vector<Road> &roads) {
    for(auto road = roads.begin(); road!=roads.end(); road++)
      road->Draw();
  }
};

#endif  // ROAD_H