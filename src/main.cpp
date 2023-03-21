#include <raylib-cpp.hpp>

#include "Configs.h"
#include "modules/Castle/Castle.h"
#include "modules/Game/Game.h"
#include "modules/Warrior/Warrior.h"
#include "modules/Game/Game.h"

#include <iostream>
#include <vector>
#include <iterator>

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: 
//    - Make new module for control attacks, retreats, castle updates
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
  int screenWidth = 1000;
  int screenHeight = 1200;

  raylib::Color textColor(LIGHTGRAY);
  raylib::Window window(screenWidth, screenHeight, "Castle Wars");
  
  SetTargetFPS(60);

  Game game = Game();

  while (!window.ShouldClose()) {
      
      game.Update();

      BeginDrawing();
        ClearBackground(CW_MAIN_BG);
        game.Draw();
      EndDrawing();

  }

  return 0;
}