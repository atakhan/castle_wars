#include "Game.h"

namespace CW {

Game::Game(int w, int h, const std::string& title, RulesBook *rules) 
    : window_(new raylib::Window(w, h, title))
    , screen_width_(w)
    , screen_height_(h)
    , objects_(new GameObjects())
    , fight_(new Fight())
    , control_(new Control())
    , menu_(new Menu())
    , rules_(rules)
{
  game_state_ = GAME_MENU_STATE;
  objects_->AddCastle(Castle({300, 300}, CW_BLUE, PLAYER,  0));
  objects_->AddCastle(Castle({700, 800}, CW_RED,  AI,      3));
  objects_->AddCastle(Castle({550, 200}, CW_RED,  AI,      0));
  objects_->AddCastle(Castle({150, 100}, GRAY,    NEUTRAL, 1));
}

bool Game::WindowShouldClose() {
  return window_->ShouldClose();
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Update() {
  if (game_state_ == GAME_PLAY_STATE) {
    control_->Update(objects_);
    fight_->Update(objects_);
    objects_->Update();
  } else if (game_state_ == GAME_MENU_STATE) {
    menu_->Update(game_state_);
  }
}

void Game::Draw() {
  if (game_state_ == GAME_PLAY_STATE) {
    DrawStats();
    control_->Draw();
    objects_->Draw();
  } else if (game_state_ == GAME_MENU_STATE) {
    menu_->Draw();
  }
}

}  // namespace CW
