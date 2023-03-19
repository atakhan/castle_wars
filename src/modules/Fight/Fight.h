#ifndef FIGHT_H
#define FIGHT_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include "../Castle/Castle.h"
#include <raylib-cpp.hpp>

class Fight
{
public:
  bool UnitsCollider(Warrior &warrior, Castle &castle);
  void Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors);
};

#endif  // FIGHT_H