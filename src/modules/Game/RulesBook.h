#ifndef CASTLE_WARS_SRC_GAME_RULESBOOK_H
#define CASTLE_WARS_SRC_GAME_RULESBOOK_H

#include <vector>

namespace CW {

class RulesBook {
 public:
  RulesBook(const std::vector<std::vector<int>> &level_rules) 
    : levels_(level_rules) {};
  
  std::vector<int> GetLevelRules(int level) { return levels_[level]; }

  int GetMaxLevel() { return levels_.size(); }

 private:
  std::vector<std::vector<int>> levels_;

};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_GAME_RULESBOOK_H