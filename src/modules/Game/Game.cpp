#include "Game.h"

namespace CW {

Game::Game(int w, int h, const std::string& title, const std::vector<std::vector<int>> &rules) 
    : window_(new raylib::Window(w, h, title))
    , screen_width_(w)
    , screen_height_(h)
    , fight_(new Fight())
    , control_(new Control())
    , menu_(new Menu())
    , rules_(rules)
{
  game_state_ = GAME_MENU_STATE;
  castles_.push_back(Castle({300, 300}, CW_BLUE, PLAYER,  0));
  castles_.push_back(Castle({700, 800}, CW_RED,  AI,      3));
  castles_.push_back(Castle({550, 200}, CW_RED,  AI,      0));
  castles_.push_back(Castle({150, 100}, GRAY,    NEUTRAL, 1));
}

bool Game::WindowShouldClose() {
  return window_->ShouldClose();
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Update() {
  if (game_state_ == GAME_PLAY_STATE) {
    control_->Update(castles_, roads_);
    fight_->Update(castles_, warriors_, roads_);
    Castle::UpdateAll(castles_, warriors_);
    Warrior::UpdateAll(warriors_);
  } else if (game_state_ == GAME_MENU_STATE) {
    menu_->Update(game_state_);
  }
}

void Game::Draw() {
  if (game_state_ == GAME_PLAY_STATE) {
    DrawStats();
    Road::DrawAll(roads_);
    Castle::DrawAll(castles_);
    Warrior::DrawAll(warriors_);
    control_->Draw();
  } else if (game_state_ == GAME_MENU_STATE) {
    menu_->Draw();
  }
}

}  // namespace CW
