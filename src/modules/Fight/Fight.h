#ifndef FIGHT_H
#define FIGHT_H

#include <raylib-cpp.hpp>

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include "../Game/GameObjects.h"

namespace CW {

class Fight
{
public:
  bool WarriorCastleCollider(Warrior &warrior, Castle &castle);
  bool WarriorWarriorCollider(Warrior &warrior, Warrior &warrior2);
  void Update(GameObjects *objects);
};

}  // namespace CW

#endif  // FIGHT_H