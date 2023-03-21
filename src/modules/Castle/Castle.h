#ifndef CASTLE_H
#define CASTLE_H

#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
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
  std::vector<Vector2> targets;
  Fraction fraction;
  CastleStatus status;
  int regen_speed;
  int regen_tick;
  int attack_speed;
  int attack_tick;
  int maxWarriors;

  Castle();
  Castle(Vector2 pos, Color color, Fraction fraction);

  void Attack(std::vector<Warrior> &warriors);
  void ResetCastle(std::vector<Road> &roads);
  void TakeDamage(Warrior &warrior, std::vector<Road> &roads);
  void AssignATarget(Vector2 mousePos, std::vector<Castle> &castles, std::vector<Road> &roads);
  void CancelAttack(); // TODO
  void HandleMouse(std::vector<Castle> &castles, std::vector<Road> &roads);
  void Regen();
  void Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors, std::vector<Road> &roads);
  void Draw();

  static void DrawAll(std::vector<Castle> &castles) {
    for(auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Draw();
  }
  
  static void UpdateAll(std::vector<Castle> &castles, std::vector<Warrior> &warriors, std::vector<Road> &roads) {
    for(auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Update(castles, warriors, roads);
  }

};

#endif  // CASTLE_H