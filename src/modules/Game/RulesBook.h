#ifndef CASTLE_WARS_SRC_GAME_RULESBOOK_H
#define CASTLE_WARS_SRC_GAME_RULESBOOK_H

#include <vector>
#include <iostream>

namespace CW {

enum LevelParameters {
  LEVEL_COST,
  LEVEL_MAX_UNIT,
  LEVEL_REGEN,
  LEVEL_ATTACK_FREQUENCY,
};

class RulesBook {
 public:
  RulesBook(const std::vector<std::vector<int>> &level_rules);
  
  std::vector<int> GetLevelRules(int level);
  int GetMaxLevel();
  int GetNextLevelCost(int current);
  int GetCurrentMax(int current);
  int GetCurrentRegen(int current);
  int GetCurrentAttackFrequency(int current);
  int GetMaxLevel(int current);

 private:
  std::vector<std::vector<int>> levels_;

};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_GAME_RULESBOOK_H