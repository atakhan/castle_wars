#include "Fight.h"
#include <iostream>

bool Fight::WarriorCastleCollider(Warrior &warrior, Castle &castle) {
  if (fabs(
    (warrior.homeCastle.x - castle.pos.x) * (warrior.homeCastle.x - castle.pos.x) + 
    (warrior.homeCastle.y - castle.pos.y) * (warrior.homeCastle.y - castle.pos.y) 
  ) > (
    (warrior.radius + castle.radius) * (warrior.radius + castle.radius)
  )) {
    return fabs(
      (warrior.pos.x - castle.pos.x) * (warrior.pos.x - castle.pos.x) + 
      (warrior.pos.y - castle.pos.y) * (warrior.pos.y - castle.pos.y) 
    ) <= (
      (warrior.radius + castle.radius) * (warrior.radius + castle.radius)
    );
  }

  return false;  
}

bool Fight::WarriorWarriorCollider(Warrior &warrior, Warrior &warrior2) {
  if (warrior.fraction != warrior2.fraction) {
    return fabs(
      (warrior.pos.x - warrior2.pos.x) * (warrior.pos.x - warrior2.pos.x) + 
      (warrior.pos.y - warrior2.pos.y) * (warrior.pos.y - warrior2.pos.y) 
    ) <= (
      (warrior.radius + warrior2.radius) * (warrior.radius + warrior2.radius)
    );
  }

  return false;  
}

void Fight::Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors, std::vector<Road> &roads) {
  for (auto castle = castles.begin(); castle != castles.end(); castle++) {
    for (auto warrior = warriors.begin(); warrior != warriors.end();) {
      if (WarriorCastleCollider(*warrior, *castle)) {
        castle->TakeDamage(*warrior, roads);
        warrior = warriors.erase(warrior);
      } else {
        ++warrior;
      }
    }
  }

  bool isErased = false;
  for (auto warrior = warriors.begin(); warrior != warriors.end();) {
    for (auto warrior2 = warriors.begin(); warrior2 != warriors.end();) {
      if (WarriorWarriorCollider(*warrior, *warrior2)) {
        warrior2 = warriors.erase(warrior2);
        isErased = true;
      } else {
        ++warrior2;
      }
    }
    if (isErased) {
      warrior = warriors.erase(warrior);
    } else {
      warrior++;
      isErased = false;
    }
  }

}