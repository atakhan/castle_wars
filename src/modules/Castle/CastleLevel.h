#ifndef CASTLELEVEL_H
#define CASTLELEVEL_H

#include "../../Configs.h"
#include "../Game/RulesBook.h"

#include <raylib-cpp.hpp>
namespace CW {

enum LevelParameters {
  LEVEL_COST,
  LEVEL_MAX_UNIT,
  LEVEL_REGEN,
  LEVEL_ATTACK_FREQUENCY,
};

class CastleLevel {
 public:
  CastleLevel(int level, RulesBook *rules);

  int GetCurrentLevel();
  int GetNextLevelCost();
  int GetCurrentMax();
  int GetCurrentRegen();
  int GetCurrentAttackFrequency();
  int GetMaxLevel();

  void SetCurrentLevel(int);

 private:
  // 1 - Cost
  // 2 - Max HP
  // 3 - Regen
  // 4 - Attack Frequency
  RulesBook *rules_;
  int current_;
  
};

}  // namespace CW

#endif  // CASTLELEVEL_H
