#ifndef WARRIOR_H
#define WARRIOR_H

#include "../../Configs.h"

#include <raylib-cpp.hpp>
#include <iostream>
#include <limits>

namespace CW {

class Warrior
{
public:
  int radius;
  Vector2 pos;
  Vector2 vel;
  Color color;
  float speed;
  float angle;
  Vector2 target;
  Vector2 homeCastle;
  Fraction fraction;
  bool targetReached;

  Warrior();
  Warrior(Vector2 pos, Vector2 target, Color color, Fraction fraction);

  void Move();
  bool CastleReached();

  void Update();
  void Draw();
};

}  // namespace CW

#endif  // WARRIOR_H