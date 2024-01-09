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
  Warrior();
  Warrior(Vector2 pos, Vector2 target, Color color, Fraction fraction);

  void Update();
  void Draw();

  void Move();
  bool CastleReached();

  int GetRadius();
  Vector2 GetPosition();
  Vector2 GetHomeCastlePosition();
  Fraction GetFraction();
  Color GetColor();


 private:
  int radius_;
  Vector2 position_;
  Vector2 velocity_;
  Color color_;
  float speed_;
  float angle_;
  Vector2 target_;
  Vector2 home_castle_;
  Fraction fraction_;
  bool target_reached_;
};

}  // namespace CW

#endif  // WARRIOR_H