#include "Fight.h"
#include <iostream>

namespace CW {

bool Fight::WarriorCastleCollider(Warrior &warrior, Castle &castle) {
  if (fabs(
    (warrior.GetHomeCastlePosition().x - castle.GetPosition().x) * (warrior.GetHomeCastlePosition().x - castle.GetPosition().x) + 
    (warrior.GetHomeCastlePosition().y - castle.GetPosition().y) * (warrior.GetHomeCastlePosition().y - castle.GetPosition().y) 
  ) > (
    (warrior.GetRadius() + castle.GetRadius()) * (warrior.GetRadius() + castle.GetRadius())
  )) {
    return fabs(
      (warrior.GetPosition().x - castle.GetPosition().x) * (warrior.GetPosition().x - castle.GetPosition().x) + 
      (warrior.GetPosition().y - castle.GetPosition().y) * (warrior.GetPosition().y - castle.GetPosition().y) 
    ) <= (
      (warrior.GetRadius() + castle.GetRadius()) * (warrior.GetRadius() + castle.GetRadius())
    );
  }

  return false;  
}

bool Fight::WarriorWarriorCollider(Warrior &warrior, Warrior &warrior2) {
  if (warrior.GetFraction() != warrior2.GetFraction()) {
    return fabs(
      (warrior.GetPosition().x - warrior2.GetPosition().x) * (warrior.GetPosition().x - warrior2.GetPosition().x) + 
      (warrior.GetPosition().y - warrior2.GetPosition().y) * (warrior.GetPosition().y - warrior2.GetPosition().y) 
    ) <= (
      (warrior.GetRadius() + warrior2.GetRadius()) * (warrior.GetRadius() + warrior2.GetRadius())
    );
  }

  return false;  
}

void Fight::Update(GameObjects *objects) {
  for (auto castle = objects->GetCastles().begin(); castle != objects->GetCastles().end(); castle++) {
    for (auto warrior = objects->GetWarriors().begin(); warrior != objects->GetWarriors().end();) {
      if (WarriorCastleCollider(*warrior, *castle)) {
        castle->TakeDamage(*warrior, objects->GetRoads());
        warrior = objects->GetWarriors().erase(warrior);
      } else {
        ++warrior;
      }
    }
  }

  bool isErased = false;
  for (auto warrior = objects->GetWarriors().begin(); warrior != objects->GetWarriors().end();) {
    for (auto warrior2 = objects->GetWarriors().begin(); warrior2 != objects->GetWarriors().end();) {
      if (WarriorWarriorCollider(*warrior, *warrior2)) {
        warrior2 = objects->GetWarriors().erase(warrior2);
        isErased = true;
      } else {
        ++warrior2;
      }
    }
    if (isErased) {
      warrior = objects->GetWarriors().erase(warrior);
    } else {
      warrior++;
      isErased = false;
    }
  }

}

}  // namespace CW
