#ifndef CASTLE_WARS_SRC_MODULES_CASTLE_CASTLE_H
#define CASTLE_WARS_SRC_MODULES_CASTLE_CASTLE_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "../Game/RulesBook.h"

#include <raylib-cpp.hpp>
#include <iostream>
#include <math.h>

namespace CW {

class Castle {
 public:
  bool is_current_;

  Castle();
  Castle(Vector2 pos, Color color, Fraction fraction, int level, RulesBook *rules);

  void Update(std::vector<Warrior> &warriors);
  void Draw();

  // Getters
  int GetCurrentLevel();
  int GetRadius();
  int GetNextLevelCost();
  bool GetMenuVisible();
  Vector2 GetPosition();
  Fraction GetFraction();

  // Setters
  void SetMenuVisible(bool value);

  bool CheckCollisionWithMouse(Vector2 mouse_pos);

  float CalculateRadiusByLevel();
  void Attack(std::vector<Warrior> &warriors);
  void TryToAssignATarget(Vector2 mousePos, std::vector<Castle> &castles, std::vector<Road> &roads);
  void TryToCancelAttack(Vector2 target);
  void TakeDamage(Warrior &warrior, std::vector<Road> &roads);
  void RemoveActiveRoads(std::vector<Road> &roads);
  void Regen();
  void Upgrade();
  void UpdateParameters();
  bool OnMenuPressed();
 
 private:
  RulesBook *rules_;
  std::vector<Vector2> targets_;
  Fraction fraction_;
  Color color_;
  Vector2 position_;
  Vector2 menu_position_;
  CastleState state_;
  float radius_;
  float menu_radius_;
  int current_level_;
  int warriors_count_;
  int max_warriors_;
  int regen_speed_;
  int regen_tick_;
  int attack_frequency_;
  int attack_tick_;
  bool show_menu_;

  bool RoadIsset(Vector2 endPos, std::vector<Road> &roads);
  void ChangeFraction(Warrior &warrior, std::vector<Road> &roads);
  void DrawAttackPath();
  void DrawCastleLevel();
  void DrawCastle();
  void DrawMenu();
};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_MODULES_CASTLE_CASTLE_H