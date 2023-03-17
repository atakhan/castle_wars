#include "GamePlayUi.h"

GamePlayUi::GamePlayUi() {
  orderBtn = {20, 1020, 200, 60};
}

void GamePlayUi::Update() {}

bool GamePlayUi::IsOrderButtonPressed() {
  if (CheckCollisionPointRec(GetMousePosition(), orderBtn)) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      return true;
    }
  }
  return false;
}

// void GamePlayUi::DrawOrderButton(Warrior &warrior) {
//   Color color;
//   std::string text;
//   Vector2 margin;
  
//   if (warrior.status == ATTACK) {
//     color = GRAY;
//     text.clear();
//     text.assign("RETREAT!");
//     margin = {-18, -15};
//   } else {
//     color = (Color) CW_RED;
//     text.clear();
//     text.assign("ATTACK!");
//     margin = {-36, -15};
//   }
  
//   DrawRectanglePro(orderBtn, {0,0}, 0, color);
//   raylib::DrawTextPro(
//     GetFontDefault(), text, 
//     {orderBtn.x, orderBtn.y},
//     margin, 0.0f, 30.0f, 5.0f,
//     WHITE
//   );
// }

// void GamePlayUi::DrawRetreatButton() {
//   DrawRectanglePro(orderBtn, {0,0}, 0, GRAY);
//   DrawTextPro(
//     GetFontDefault(), "RETREAT!", 
//     {orderBtn.x, orderBtn.y},
//     {-18, -15}, 0.0f, 30.0f, 5.0f,
//     WHITE
//   );
// }

// void GamePlayUi::Draw(Warrior &warrior) {
//   DrawRectangle(0, 1000, 1000, 200, CW_MENU_LINE_COLOR);
//   DrawLineEx({0, 1000}, {1000, 1000}, 2.0f, CW_MENU_BG_COLOR);
//   DrawOrderButton(warrior);
// }
