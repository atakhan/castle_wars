#include "Control.h"

namespace CW {

Control::Control() {
  is_hatching_ = false;
  is_castle_pressed_ = false;
}

void Control::CancelAttack(GameObjects *objects, Vector2 mouse_pressed_position, Vector2 mouse_released_position) {
  for (auto road = objects->GetRoads().begin(); road != objects->GetRoads().end();)
  {
    if (road->fraction == PLAYER && Control::isIntersecting(road->path[0], road->path[1], mouse_pressed_position, mouse_released_position)) {
      for (auto castle = objects->GetCastles().begin(); castle != objects->GetCastles().end(); castle++) {
        if (abs(road->path[0].x - castle->GetPosition().x) < 1 && abs(road->path[0].y - castle->GetPosition().y) < 1) {
          castle->TryToCancelAttack(road->path[1]);
        }
      }
      road = objects->GetRoads().erase(road);
    } else {
      road++;
    }
  }
}

void Control::CheckCastlePressed(GameObjects *objects) {
  for (auto castle = objects->GetCastles().begin(); castle != objects->GetCastles().end(); castle++) {
    if (castle->GetFraction() == PLAYER) {
      if (CheckCollisionPointCircle(mouse_pressed_position_, castle->GetPosition(), castle->GetRadius())) {
        castle->is_current_ = true;
        is_castle_pressed_ = true;
        break;
      } else {
        castle->is_current_ = false;
        castle->SetMenuVisible(false);
      }
    }
  }
  if (!is_castle_pressed_) {
    is_hatching_ = true;
  }
}
  
void Control::LookForAnAttackingCastle(GameObjects *objects) {
  for (auto castle = objects->GetCastles().begin(); castle != objects->GetCastles().end(); castle++) {
    if (castle->GetFraction() == PLAYER && castle->is_current_) {
      // show menu
      if (CheckCollisionPointCircle(mouse_released_position_, castle->GetPosition(), castle->GetRadius())) {
        castle->is_current_ = false;
        castle->SetMenuVisible(true);
      // assign a target
      } else {
        castle->TryToAssignATarget(
          mouse_released_position_, 
          objects->GetCastles(), 
          objects->GetRoads()
        );
      }
      is_castle_pressed_ = false;
      break;
    } 
  }
}

void Control::Update(GameObjects *objects) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {  
    mouse_pressed_position_ = GetMousePosition();
    CheckCastlePressed(objects);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    mouse_released_position_ = GetMousePosition();
    if (is_castle_pressed_) {
      LookForAnAttackingCastle(objects);
    } else {
      is_hatching_ = false;
      CancelAttack(objects, mouse_pressed_position_, mouse_released_position_);
    }
  }
}

void Control::Draw() {
  if (is_hatching_) {
    DrawLineEx(mouse_pressed_position_, GetMousePosition(), 10.0f, GREEN);
  }
}

}  // namespace CW
