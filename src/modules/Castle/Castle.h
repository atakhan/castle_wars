#ifndef CASTLE_H
#define CASTLE_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "../Level/Level.h"

#include <raylib-cpp.hpp>
#include <iostream>
#include <math.h>

class Castle
{
public:
  Level level;
  Fraction fraction;
  Color color;
  Vector2 pos;
  Vector2 menuPos;
  float radius;
  float menuRadius;
  bool isCurrent;
  bool showMenu;
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
  void TryToCancelAttack(Vector2 target);
  bool RoadIsset(Vector2 endPos, std::vector<Road> &roads);
  void TakeDamage(Warrior &warrior, std::vector<Road> &roads);
  void RemoveActiveRoads(std::vector<Road> &roads);
  void Regen();
  void UpgradeCastle();
  void Update(std::vector<Warrior> &warriors);
  void DrawCastle();
  void DrawAttackPath();
  void DrawMenu();
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