#include "RulesBook.h"

namespace CW {

RulesBook::RulesBook(const std::vector<std::vector<int>> &level_rules) 
    : levels_(level_rules) {};
  
std::vector<int> RulesBook::GetLevelRules(int level) {
  return levels_[level];
}

int RulesBook::GetMaxLevel() {
  return levels_.size();
}

int RulesBook::GetNextLevelCost(int current) {
  if (current + 1 < levels_.size()) {
    return levels_.at(current+1).at(LEVEL_COST);
  } else {
    return 99999;
  }
}

int RulesBook::GetCurrentMax(int current) {
  return levels_.at(current).at(LEVEL_MAX_UNIT);
}

int RulesBook::GetCurrentRegen(int current) {
  return levels_.at(current).at(LEVEL_REGEN);
}

int RulesBook::GetCurrentAttackFrequency(int current) {
  return levels_.at(current).at(LEVEL_ATTACK_FREQUENCY);
}

}  // namespace CW
