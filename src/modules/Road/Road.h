#ifndef ROAD_H
#define ROAD_H

#include "../../Configs.h"
#include "../Milestone/Milestone.h"
#include <raylib-cpp.hpp>
#include <iostream>

class Road
{
public:
  std::vector<Milestone> path;

  Road();
  Road(std::vector<Milestone> path);

  void Draw();
};

#endif  // ROAD_H