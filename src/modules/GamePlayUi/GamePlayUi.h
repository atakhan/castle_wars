#ifndef GAMEPLAYUI_H
#define GAMEPLAYUI_H

#include "../../Colors.h"
#include "../Warrior/Warrior.h"
#include <raylib-cpp.hpp>
#include <iostream>

class GamePlayUi
{
public:
  Vector2 pos;
  Color color;
  float width;
  float height;
  Rectangle attackBtn;

  GamePlayUi();

  bool IsAttackButtonPressed();
  void Update();
  void Draw();

};

#endif  // GAMEPLAYUI_H