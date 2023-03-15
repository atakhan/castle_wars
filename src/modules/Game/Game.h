#ifndef GAME_H
#define GAME_H

#include "../Warrior/Warrior.h"
#include <raylib-cpp.hpp>

class Game {
public:
  Warrior warrior;

  Game();

  void DrawStats();
  void Draw();
  void Update();

};

#endif  // GAME_H