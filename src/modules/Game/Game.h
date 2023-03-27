#ifndef GAME_H
#define GAME_H

#include "../../Configs.h"
#include "../Control/Control.h"
#include "../Castle/Castle.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "../Fight/Fight.h"
#include "../Menu/Menu.h"

#include <raylib-cpp.hpp>

class Game {
public:
  GameState gameState;
  Fight fight;
  Control control;
  Menu menu;
  std::vector<Road> roads;
  std::vector<Castle> castles;
  std::vector<Warrior> warriors;

  Game();

  void Update();
  
  void DrawStats();
  void Draw();

};

#endif  // GAME_H
