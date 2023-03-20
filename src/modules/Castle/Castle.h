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
  bool production;
  bool isCurrent;
  float radius;
  int warriorsCount;
  Color color;
  Vector2 pos;
  Vector2 target;
  Fraction fraction;
  CastleStatus status;
  int regen;
  int tick;
  int maxWarriors;

  Castle();
  Castle(Vector2 pos, Color color, Fraction fraction);

  void Attack(std::vector<Warrior> &warriors);
  void TakeDamage(Warrior &warrior);
  void AssignATarget(std::vector<Castle> &castles);
  void Regen();
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