#ifndef CASTLE_WARS_SRC_MODULES_CASTLE_CASTLE_H
#define CASTLE_WARS_SRC_MODULES_CASTLE_CASTLE_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "CastleMenu.h"
#include "CastleLevel.h"

#include <raylib-cpp.hpp>
#include <iostream>
#include <math.h>

namespace CW {

class Castle {
 public:
  bool is_current_;

  Castle();
  Castle(Vector2 pos, Color color, Fraction fraction, int level);

  int GetCurrentLevel();
  int GetRadius();
  Fraction GetFraction();
  Vector2 GetPosition();
  int GetNextLevelCost();

  float CalculateRadiusByLevel();
  void Attack(std::vector<Warrior> &warriors);
  void TryToAssignATarget(Vector2 mousePos, std::vector<Castle> &castles, std::vector<Road> &roads);
  void TryToCancelAttack(Vector2 target);
  bool RoadIsset(Vector2 endPos, std::vector<Road> &roads);
  void TakeDamage(Warrior &warrior, std::vector<Road> &roads);
  void RemoveActiveRoads(std::vector<Road> &roads);
  void Regen();
  void Upgrade();
  void UpdateParameters();
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
 
 private:
  std::vector<Vector2> targets_;
  CastleMenu *menu_;
  CastleLevel *level_;
  Fraction fraction_;
  Color color_;
  Vector2 position_;
  CastleState state_;
  float radius_;
  int warriors_count_;
  int max_warriors_;
  int regen_speed_;
  int regen_tick_;
  int attack_frequency_;
  int attack_tick_;

  void ChangeFraction(const Warrior &warrior, std::vector<Road> &roads);
  void DrawAttackPath();
  void DrawCastle();
  void DrawMenu();
};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_MODULES_CASTLE_CASTLE_H