#ifndef CASTLE_WARS_SRC_GAME_GAME_H
#define CASTLE_WARS_SRC_GAME_GAME_H

#include <vector>

#include <raylib-cpp.hpp>

#include "../../Configs.h"
#include "../Control/Control.h"
#include "../Fight/Fight.h"
#include "../Menu/Menu.h"
#include "RulesBook.h"
#include "GameObjects.h"

namespace CW {

class Game {
 public:
  Game(int w, int h, const std::string& title, RulesBook *rules);
  void Update();
  void Draw();
  void DrawStats();
  bool WindowShouldClose();

 private:
  GameState game_state_;
  raylib::Window *window_;
  Fight *fight_;
  Control *control_;
  Menu *menu_;
  GameObjects *objects_;
  RulesBook *rules_;

  int screen_width_;
  int screen_height_;
};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_GAME_GAME_H
