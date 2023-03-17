#ifndef GAME_H
#define GAME_H

#include "../../Colors.h"
#include "../Warrior/Warrior.h"
#include "../Army/Army.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include "../GamePlayUi/GamePlayUi.h"
#include <raylib-cpp.hpp>

enum GameState {
  GAME_MENU_STATE,
  GAME_PLAY_STATE,
  GAME_OVER_STATE
};

class Game {
public:
  Road road;
  GamePlayUi ui;
  std::vector<Castle> castles;
  std::vector<Army> armies;
  std::vector<Milestone> path;

  Game();

  void GenerateCastles();
  void GeneratePath();
  void DrawStats();
  void Draw();
  void Update();

};

#endif  // GAME_H
