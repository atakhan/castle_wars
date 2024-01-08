#include "Menu.h"

namespace CW {

Menu::Menu() {
  startGameBtn = { 
    GetScreenWidth() / 2.0f, 
    GetScreenHeight() / 2.0f,
    310, 60
  };
  exitGameBtn = { 
    GetScreenWidth() / 2.0f, 
    GetScreenHeight() / 2.0f + 80.0f,
    270, 60
  };
  hoverColor = ColorAlpha(RED, 0.3f);
}

void Menu::Update(GameState &gameState) {
  // start game btn
  if (CheckCollisionPointRec(GetMousePosition(), startGameBtn)) {
    hoverRec = startGameBtn;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      gameState = GAME_PLAY_STATE;
    }
  }

  // exit game btn
  if (CheckCollisionPointRec(GetMousePosition(), exitGameBtn)) {
    hoverRec = exitGameBtn;
  }
}

void Menu::Draw() {
  DrawRectanglePro(
    hoverRec, 
    {0.0f, 0.0f}, 0.0f, 
    ColorAlpha(RED, 0.3f)
  );

  raylib::DrawTextPro(
    GetFontDefault(), "Start Game", {startGameBtn.x + 20, startGameBtn.y + 10},
    {0.0f, 0.0f}, 0.0f, 40.0f, 8.0f, CW_BLUE
  );

  raylib::DrawTextPro(
    GetFontDefault(), "Exit Game", {exitGameBtn.x + 20, exitGameBtn.y + 10},
    {0.0f, 0.0f}, 0.0f, 40.0f, 8.0f, CW_BLUE
  );
}

}  // namespace CW
