#ifndef GAMEPLAYUI_H
#define GAMEPLAYUI_H

#include "../../Configs.h"
#include "../Warrior/Warrior.h"
#include <raylib-cpp.hpp>
#include <iostream>
#include <string>

class GamePlayUi
{
public:
  Vector2 pos;
  Color color;
  float width;
  float height;
  Rectangle orderBtn;

  GamePlayUi();

  bool IsOrderButtonPressed();
  
  void Update();
  
  // void DrawOrderButton(Warrior &warrior);
  // void Draw(Warrior &warrior);

};

#endif  // GAMEPLAYUI_H