#ifndef CASTLE_H
#define CASTLE_H

#include "../Warrior/Warrior.h"
#include <raylib-cpp.hpp>
#include <iostream>

enum CastleStatus {
  ATTACK,
  DEFENSE
};

class Castle
{
public:
  float radius;
  Vector2 pos;
  Color color;
  unsigned int warriorsCount;
  bool production;
  Vector2 target;
  bool isCurrent;
  CastleStatus status;
  unsigned int hp;
  Fraction fraction;

  Castle();
  Castle(Vector2 pos, Color color, Fraction fraction);

  void Attack(std::vector<Warrior> &warriors);
  void AssignATarget(std::vector<Castle> &castles);
  void Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors);
  void Draw();

  static void DrawAll(std::vector<Castle> &castles) {
    for(auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Draw();
  }
  
  static void UpdateAll(std::vector<Castle> &castles, std::vector<Warrior> &warriors) {
    for(auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Update(castles, warriors);
  }

};

#endif  // CASTLE_H