#ifndef CASTLE_H
#define CASTLE_H

#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "../Level/Level.h"
#include <raylib-cpp.hpp>
#include <iostream>
#include <math.h>

enum CastleStatus {
  ATTACK,
  DEFENSE
};

class Castle
{
public:
  Level level;
  Fraction fraction;
  Color color;
  Vector2 pos;
  float radius;
  bool isCurrent;
  CastleStatus status;
  std::vector<Vector2> targets;
  int warriorsCount;
  int maxWarriors;
  int regenSpeed;
  int regenTick;
  int attackFrequency;
  int attackTick;

  Castle();
  Castle(Vector2 pos, Color color, Fraction fraction, int level);

  void GetLevelParameters();
  float CalculateRadiusByLevel();

  void Attack(std::vector<Warrior> &warriors);
  void TryToAssignATarget(Vector2 mousePos, std::vector<Castle> &castles, std::vector<Road> &roads);
  void CancelAttack(Vector2 target);
  bool RoadIsset(Vector2 endPos, std::vector<Road> &roads);
  void TakeDamage(Warrior &warrior, std::vector<Road> &roads);
  void RemoveActiveRoads(std::vector<Road> &roads);
  void Regen();
  void Update(std::vector<Warrior> &warriors);
  void Draw();

  static void DrawAll(std::vector<Castle> &castles) {
    for (auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Draw();
  }
  
  static void UpdateAll(std::vector<Castle> &castles, std::vector<Warrior> &warriors) {
    for (auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Update(warriors);
  }

};

#endif  // CASTLE_H