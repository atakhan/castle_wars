#ifndef MENU_H
#define MENU_H

#include "../../Configs.h"
// #include "../Fight/Fight.h"

#include <raylib-cpp.hpp>

class Menu {
 public:
  Rectangle startGameBtn;
  Rectangle exitGameBtn;
  Rectangle hoverRec;
  Color hoverColor;
  
  Menu();

  void Update(GameState &gameState);
  void Draw();
};

#endif  // MENU_H