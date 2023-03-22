#ifndef CONTROL_H
#define CONTROL_H

#include "../../Configs.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include <raylib-cpp.hpp>
#include <iostream>

class Control
{
public:
  Vector2 mousePressedPos;
  Vector2 mouseReleasedPos;
  bool isHatching;
  bool isCastlePressed;
  
  Control();
  
  bool isIntersecting(Vector2& p1, Vector2& p2, Vector2& q1, Vector2& q2);
  void Update(std::vector<Castle> &castles, std::vector<Road> &roads);
  void Draw();
};

#endif  // CONTROL_H