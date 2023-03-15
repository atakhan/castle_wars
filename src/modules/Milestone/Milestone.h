#ifndef MILESTONE_H
#define MILESTONE_H

#include <raylib-cpp.hpp>
#include <iostream>

class Milestone
{
public:
  Vector2 pos;
  bool reached;

  Milestone(Vector2 pos);

  void Draw();
};

#endif  // MILESTONE_H