#include "Fight.h"
#include <iostream>

bool Fight::UnitsCollider(Warrior &warrior, Castle &castle) {
  if (warrior.fraction != castle.fraction) {
      return fabs(
          (warrior.pos.x - castle.pos.x) * (warrior.pos.x - castle.pos.x) + 
          (warrior.pos.y - castle.pos.y) * (warrior.pos.y - castle.pos.y) 
      ) <= (
          (warrior.radius + castle.radius) * (warrior.radius + castle.radius)
      );
  }
  return false;
}

void Fight::Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors) {
  warriors.erase(
    std::remove_if(
        warriors.begin(), 
        warriors.end(),
        [](Warrior const & p) { return p.targetReached; }
    ), 
    warriors.end()
  );
}