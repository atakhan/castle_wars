#ifndef CASTLE_WARS_SRC_GAME_GAMEOBJECTS_H
#define CASTLE_WARS_SRC_GAME_GAMEOBJECTS_H

#include <vector>

#include "../Castle/Castle.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"

namespace CW {

class GameObjects {
 public:
  GameObjects();
  void AddCastle(const Castle &castle);
  void Update();
  void Draw();
  std::vector<Castle> &GetCastles();
  std::vector<Road> &GetRoads();
  std::vector<Warrior> &GetWarriors();


 private:
  std::vector<Road> roads_;
  std::vector<Castle> castles_;
  std::vector<Warrior> warriors_;
};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_GAME_GAMEOBJECTS_H