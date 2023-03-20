#ifndef WARRIOR_H
#define WARRIOR_H

#include "../../Configs.h"

#include <raylib-cpp.hpp>
#include <iostream>
#include <limits>

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

  static void UpdateAll(std::vector<Warrior> &warriors) {
    for(auto warrior = warriors.begin(); warrior != warriors.end(); warrior++)
      warrior->Update();
  }

  static void DrawAll(std::vector<Warrior> &warriors) {
    for(auto warrior = warriors.begin(); warrior != warriors.end(); warrior++)
      warrior->Draw();
  }

};

#endif  // WARRIOR_H