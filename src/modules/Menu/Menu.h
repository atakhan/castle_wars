#ifndef MENU_H
#define MENU_H

#include "../../Configs.h"

#include <raylib-cpp.hpp>

namespace CW {

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

}  // namespace CW

#endif  // MENU_H