#include "Control.h"

namespace CW {

Control::Control() {
  is_hatching_ = false;
  is_castle_pressed_ = false;
}

void Control::CancelAttack(std::vector<Castle> &castles, std::vector<Road> &roads, Vector2 mouse_pressed_position, Vector2 mouse_released_position) {
  for (auto road = roads.begin(); road!=roads.end();) {    
    if (road->fraction == PLAYER && Control::isIntersecting(road->path[0], road->path[1], mouse_pressed_position, mouse_released_position)) {
      for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
        if (abs(road->path[0].x - castle->GetPosition().x) < 1 && abs(road->path[0].y - castle->GetPosition().y) < 1) {
          castle->TryToCancelAttack(road->path[1]);
        }
      }
      road = roads.erase(road);
    } else {
      road++;
    }
  }
}

void Control::CheckCastlePressed(std::vector<Castle> &castles) {
  for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
    if (castle->GetFraction() == PLAYER) {
      if (CheckCollisionPointCircle(mouse_pressed_position_, castle->GetPosition(), castle->GetRadius())) {
        castle->is_current_ = true;
        is_castle_pressed_ = true;
        break;
      } else {
        castle->is_current_ = false;
        castle->showMenu = false;
      }
    }
  }
  if (!is_castle_pressed_) {
    is_hatching_ = true;
  }
}
  
void Control::LookForAnAttackingCastle(std::vector<Castle> &castles, std::vector<Road> &roads) {
  for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
    if (castle->GetFraction() == PLAYER && castle->is_current_) {
      // show menu
      if (CheckCollisionPointCircle(mouse_released_position_, castle->GetPosition(), castle->GetRadius())) {
        castle->is_current_ = false;
        castle->showMenu = true;
      // assign a target
      } else {
        castle->TryToAssignATarget(mouse_released_position_, castles, roads);
      }
      is_castle_pressed_ = false;
      break;
    } 
  }
}

void Control::Update(std::vector<Castle> &castles, std::vector<Road> &roads) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {  
    mouse_pressed_position_ = GetMousePosition();
    CheckCastlePressed(castles);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    mouse_released_position_ = GetMousePosition();
    if (is_castle_pressed_) {
      LookForAnAttackingCastle(castles, roads);
    } else {
      is_hatching_ = false;
      CancelAttack(castles, roads, mouse_pressed_position_, mouse_released_position_);
    }
  }
}

void Control::Draw() {
  if (is_hatching_) {
    DrawLineEx(mouse_pressed_position_, GetMousePosition(), 10.0f, GREEN);
  }
}

}  // namespace CW
