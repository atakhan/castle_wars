#include <vector>
#include <iterator>

#include <raylib-cpp.hpp>

#include "Configs.h"
#include "modules/Castle/Castle.h"
#include "modules/Game/Game.h"
#include "modules/Warrior/Warrior.h"
#include "modules/Game/RulesBook.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DONE: 
//    - Make new module for control attacks, retreats, castle updates
//    - Deny to make more than 1 road to same enemy castle
//    - delete target from castle targets after cancel attack
//    - Fix bug: if one path deleted other path stop attack
//    - Add update castle feature
//    - Fix bug: in last level regen stopped
//    - Add cost of update to update button
//    - Add neutral castles (without regen)
//    - BUG: crash on upgrade castle when it max level

////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TODO:
//    - Add maps
//    - Add rules editor before game started
//    - Add go back to menu
//    - Make regen stop if castle attacked 
//    - Make castle state view (idle, attack, defense)
//    - Add regen icon to castles

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
  // 1 - Cost
  // 2 - Max HP
  // 3 - Regen
  // 4 - Attack Frequency
  CW::RulesBook rules({
    {0,  12, 80, 40},
    {2,  24, 70, 35},
    {2, 48, 60, 30},
    {2, 96, 50, 25},
  });

  CW::Game game(
    1000, 1200, 
    "Castle Wars 0.1",
    &rules
  );

  SetTargetFPS(60);

  while (!game.WindowShouldClose())
  {    
    game.Update();

    BeginDrawing();
    ClearBackground(CW_MAIN_BG);
    
    game.Draw();
    
    EndDrawing();

  }

  return 0;
}