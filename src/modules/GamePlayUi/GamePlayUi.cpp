#include "GamePlayUi.h"

GamePlayUi::GamePlayUi() {
  attackBtn = {20, 1020, 100, 40};
}

void GamePlayUi::Update() {}

bool GamePlayUi::IsAttackButtonPressed() {
  if (CheckCollisionPointRec(GetMousePosition(), attackBtn)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      return true;
    }
  }
  return false;
}

void GamePlayUi::Draw() {
  DrawRectangle(0, 1000, 1000, 200, CW_MENU_LINE_COLOR);
  DrawLineEx({0, 1000}, {1000, 1000}, 2.0f, CW_MENU_BG_COLOR);
  // DrawRectangle(20, 1020, 100, 40, CW_RED);
  DrawRectanglePro(attackBtn, {0,0}, 0, CW_RED);
}
