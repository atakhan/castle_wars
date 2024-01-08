#ifndef FIGHT_H
#define FIGHT_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include <raylib-cpp.hpp>

namespace CW {

class Fight
{
public:
  bool WarriorCastleCollider(Warrior &warrior, Castle &castle);
  bool WarriorWarriorCollider(Warrior &warrior, Warrior &warrior2);
  void Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors, std::vector<Road> &roads);
};

}  // namespace CW

#endif  // FIGHT_H