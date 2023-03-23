#ifndef LEVEL_H
#define LEVEL_H

#include "../../Configs.h"

#include <raylib-cpp.hpp>

enum LevelParameters {
  LEVEL_COST,
  LEVEL_MAX_UNIT,
  LEVEL_REGEN,
  LEVEL_ATTACK_FREQUENCY,
};

class Level {
 public:
  // 1 - Cost
  // 2 - Max
  // 3 - Regen
  // 4 - Attack Frequency
  // 1   2   3   4
  const int map[8][4] = {
    {0,  8,  80, 72},
    {6,  16, 72, 64},
    {12, 24, 64, 56},
    {18, 32, 56, 48},
    {24, 40, 48, 40},
    {30, 48, 40, 32},
    {36, 56, 32, 24},
    {42, 64, 24, 16}
  };

  int current;

  Level();
  Level(int level);

  int getCurrentLevel();
  int getNextLevelCost();
  int getCurrentMax();
  int getCurrentRegen();
  int getCurrentAttackFrequency();
};

#endif  // LEVEL_H
