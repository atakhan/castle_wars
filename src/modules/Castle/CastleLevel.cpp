#include "CastleLevel.h"

namespace CW {

CastleLevel::CastleLevel(int level, RulesBook *rules) 
    : current_(level)
    , rules_(rules)
{}

int CastleLevel::GetCurrentLevel() {
  return current_;
}

int CastleLevel::GetNextLevelCost() {
  return rules_->GetLevelRules(current_ + 1).at(LEVEL_COST);
}

int CastleLevel::GetCurrentMax() {
  return rules_->GetLevelRules(current_).at(LEVEL_MAX_UNIT);
}

int CastleLevel::GetCurrentRegen() {
  return rules_->GetLevelRules(current_).at(LEVEL_REGEN);
}

int CastleLevel::GetCurrentAttackFrequency() {
  return rules_->GetLevelRules(current_).at(LEVEL_ATTACK_FREQUENCY);
}

int CastleLevel::GetMaxLevel() {
  return rules_->GetMaxLevel();
}

void CastleLevel::SetCurrentLevel(int new_level) {
  current_ = new_level;
}

}  // namespace CW
