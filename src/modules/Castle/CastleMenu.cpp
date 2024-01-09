#include "CastleMenu.h"

namespace CW {

CastleMenu::CastleMenu(Vector2 castle_position, CastleLevel *level) 
    : position_{
        castle_position.x - (30.0f * (powf((level->GetCurrentLevel() + 1), 0.5))),
        castle_position.y + (30.0f * (powf((level->GetCurrentLevel() + 1), 0.5)))
      }
    , radius_(30.0f)
    , show_(false)
    , level_(level)
{}

void CastleMenu::Update() {}

bool CastleMenu::Show() {
  return show_;
}

void CastleMenu::Draw()
{
  DrawCircle(position_.x, position_.y, radius_, ColorAlpha(DARKGREEN, 0.85));
  std::string updateCost = std::to_string(level_->GetNextLevelCost());
  raylib::DrawText(updateCost, position_.x - 8.0f, position_.y - 8.0f, 12.0f, WHITE);
}

bool CastleMenu::UpgradeCastle() {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && 
      CheckCollisionPointCircle(GetMousePosition(), position_, radius_)) {
    return true;
  }
  return false;
}

void CastleMenu::SetVisible(bool value) {
  show_ = value;
}

}  // namespace CW