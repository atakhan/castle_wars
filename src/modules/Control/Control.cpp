#include "Control.h"

Control::Control() {
  isHatching = false;
  isCastlePressed = false;
}

void Control::CancelAttack(std::vector<Castle> &castles, std::vector<Road> &roads, Vector2 mousePressedPos, Vector2 mouseReleasedPos) {
  for (auto road = roads.begin(); road!=roads.end();) {    
    if (road->fraction == PLAYER && Control::isIntersecting(road->path[0], road->path[1], mousePressedPos, mouseReleasedPos)) {
      for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
        if (abs(road->path[0].x - castle->pos.x) < 1 && abs(road->path[0].y - castle->pos.y) < 1) {
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
    if (castle->fraction == PLAYER) {
      if (CheckCollisionPointCircle(mousePressedPos, castle->pos, castle->radius)) {
        castle->isCurrent = true;
        isCastlePressed = true;
        break;
      } else {
        castle->isCurrent = false;
        castle->showMenu = false;
      }
    }
  }
  if (!isCastlePressed) {
    isHatching = true;
  }
}
  
void Control::LookForAnAttackingCastle(std::vector<Castle> &castles, std::vector<Road> &roads) {
  for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
    if (castle->fraction == PLAYER && castle->isCurrent) {
      // show menu
      if (CheckCollisionPointCircle(mouseReleasedPos, castle->pos, castle->radius)) {
        castle->isCurrent = false;
        castle->showMenu = true;
      // assign a target
      } else {
        castle->TryToAssignATarget(mouseReleasedPos, castles, roads);
      }
      isCastlePressed = false;
      break;
    } 
  }
}

void Control::Update(std::vector<Castle> &castles, std::vector<Road> &roads) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {  
    mousePressedPos = GetMousePosition();
    CheckCastlePressed(castles);
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    mouseReleasedPos = GetMousePosition();
    if (isCastlePressed) {
      LookForAnAttackingCastle(castles, roads);
    } else {
      isHatching = false;
      CancelAttack(castles, roads, mousePressedPos, mouseReleasedPos);
    }
  }
}

void Control::Draw() {
  if (isHatching) {
    DrawLineEx(mousePressedPos, GetMousePosition(), 10.0f, GREEN);
  }
}