#ifndef GAME_H
#define GAME_H

#include "../../Configs.h"
#include "../Castle/Castle.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "../Fight/Fight.h"
#include "../GamePlayUi/GamePlayUi.h"

#include <raylib-cpp.hpp>

enum GameState {
  GAME_MENU_STATE,
  GAME_PLAY_STATE,
  GAME_OVER_STATE
};

class Game {
public:
  Fight fight;
  GamePlayUi ui;

  std::vector<Road> roads;

  std::vector<Castle> castles;
  std::vector<Warrior> warriors;

  Game();

  void GenerateCastles();
  void GeneratePath();
  void DrawStats();
  void Draw();
  void Update();

};

#endif  // GAME_H
