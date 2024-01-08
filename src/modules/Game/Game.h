#ifndef CASTLE_WARS_SRC_GAME_GAME_H
#define CASTLE_WARS_SRC_GAME_GAME_H

#include <vector>

#include <raylib-cpp.hpp>

#include "../../Configs.h"
#include "../Control/Control.h"
#include "../Castle/Castle.h"
#include "../Warrior/Warrior.h"
#include "../Road/Road.h"
#include "../Fight/Fight.h"
#include "../Menu/Menu.h"

namespace CW {

struct Rules {
  std::vector<std::vector<int>> levels_;
  
  Rules(const std::vector<std::vector<int>> &level_rules) : levels_(level_rules) {};
};

struct GameObjects {
  std::vector<Road> roads_;
  std::vector<Castle> castles_;
  std::vector<Warrior> warriors_;
};

class Game {
 public:
  Game(int w, int h, const std::string& title, const std::vector<std::vector<int>> &rules);
  void Init();
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
  GameObjects objects_;
  Rules rules_;

  int screen_width_;
  int screen_height_;
};

}  // namespace CW

#endif  // CASTLE_WARS_SRC_GAME_GAME_H
