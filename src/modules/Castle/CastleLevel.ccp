#include "CastleLevel.h"

namespace CW {

CastleLevel::CastleLevel(int level) : current_(level) {}

int CastleLevel::GetCurrentLevel() {
  return current_;
}

int CastleLevel::GetNextLevelCost() {
  return rules_[current_+1][LEVEL_COST];
}

int CastleLevel::GetCurrentMax() {
  return rules_[current_][LEVEL_MAX_UNIT];
}

int CastleLevel::GetCurrentRegen() {
  return rules_[current_][LEVEL_REGEN];
}

int CastleLevel::GetCurrentAttackFrequency() {
  return rules_[current_][LEVEL_ATTACK_FREQUENCY];
}

int CastleLevel::GetMaxLevel() {
  return rules_.size();
}

void CastleLevel::SetCurrentLevel(int new_level) {
  current_ = new_level;
}

}  // namespace CW
