#include "CastleMenu.h"

namespace CW {

CastleMenu::CastleMenu(Vector2 castle_position, int level) 
    : position_{
        castle_position.x - (30.0f * (powf((level + 1), 0.5))),
        castle_position.y + (30.0f * (powf((level + 1), 0.5)))
      }
    , radius_(30.0f)
    , show_(false)
{}

void CastleMenu::Update() {}

bool CastleMenu::Show() {
  return show_;
}

void CastleMenu::Draw()
{
  DrawCircle(menu_position.x, menu_position.y, radius_, ColorAlpha(DARKGREEN, 0.85));
  std::string updateCost = std::to_string(castle->GetNextLevelCost());
  raylib::DrawText(updateCost, menu_position.x - 8.0f, menu_position.y - 8.0f, 12.0f, WHITE);
}

bool CastleMenu::UpgradeCastle() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
      CheckCollisionPointCircle(GetMousePosition(), position_, radius_)) {
    return true;
  }
  return false;
}

}  // namespace CW