#ifndef FIGHT_H
#define FIGHT_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Castle/Castle.h"
#include <raylib-cpp.hpp>

class Fight
{
public:
  bool WarriorCastleCollider(Warrior &warrior, Castle &castle);
  bool WarriorWarriorCollider(Warrior &warrior, Warrior &warrior2);
  void Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors);
};

#endif  // FIGHT_H