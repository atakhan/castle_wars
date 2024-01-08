#ifndef CONTROL_H
#define CONTROL_H

#include "../../Configs.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include <raylib-cpp.hpp>
#include <iostream>

namespace CW {

class Control
{
 public:  
  Control();
  
  void LookForAnAttackingCastle(std::vector<Castle> &castles, std::vector<Road> &roads);
  void CheckCastlePressed(std::vector<Castle> &castles);
  void CancelAttack(std::vector<Castle> &castles, std::vector<Road> &roads, Vector2 mousePressedPos, Vector2 mouseReleasedPos);
  void Update(std::vector<Castle> &castles, std::vector<Road> &roads);
  void Draw();

  static bool isIntersecting(Vector2& p1, Vector2& p2, Vector2& q1, Vector2& q2) {
    return (
      (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
      * ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
      &&
      (((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
      * ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0)
    );
  }

 private:
  Vector2 mouse_pressed_position_;
  Vector2 mouse_released_position_;
  bool is_hatching_;
  bool is_castle_pressed_;
};

}  // namespace CW

#endif  // CONTROL_H