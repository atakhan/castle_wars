#ifndef GAME_H
#define GAME_H

#include "../../Colors.h"
#include "../Warrior/Warrior.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include "../GamePlayUi/GamePlayUi.h"
#include <raylib-cpp.hpp>

class Game {
public:
  Warrior warrior;
  Castle blueCastle;
  Castle redCastle;
  Road road;
  GamePlayUi ui;
  std::vector<Milestone> path;

  Game();

  void GeneratePath();
  void DrawStats();
  void Draw();
  void Update();

};

#endif  // GAME_H
