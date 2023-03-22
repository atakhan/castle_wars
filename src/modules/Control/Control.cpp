#include "Control.h"

Control::Control() {
  isHatching = false;
  isCastlePressed = false;
}

bool Control::isIntersecting(Vector2& p1, Vector2& p2, Vector2& q1, Vector2& q2) {
  return (
    (((q1.x - p1.x) * (p2.y - p1.y) - (q1.y - p1.y) * (p2.x - p1.x))
    * ((q2.x - p1.x) * (p2.y - p1.y) - (q2.y - p1.y) * (p2.x - p1.x)) < 0)
    &&
    (((p1.x - q1.x) * (q2.y - q1.y) - (p1.y - q1.y) * (q2.x - q1.x))
    * ((p2.x - q1.x) * (q2.y - q1.y) - (p2.y - q1.y) * (q2.x - q1.x)) < 0)
  );
}

void Control::Update(std::vector<Castle> &castles, std::vector<Road> &roads) {
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      
    mousePressedPos = GetMousePosition();

    for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
      if (castle->fraction == PLAYER) {
        if (CheckCollisionPointCircle(mousePressedPos, castle->pos, castle->radius)) {
          castle->isCurrent = true;
          isCastlePressed = true;
          break;
        } else {
          castle->isCurrent = false;
        }
      }
    }
    if (!isCastlePressed) {
      isHatching = true;
    }
  }
  
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {

    mouseReleasedPos = GetMousePosition();
    
    if (isCastlePressed) {
      for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
        if (castle->fraction == PLAYER && castle->isCurrent) {
          castle->AssignATarget(mouseReleasedPos, castles, roads);
          isCastlePressed = false;
          break;
        } 
      }
    } else {
      isHatching = false;
      for (auto road = roads.begin(); road!=roads.end();) {
          
        if (road->fraction == PLAYER 
          && isIntersecting(
            road->path[0], road->path[1],
            mousePressedPos, mouseReleasedPos)
        ) {
          for (auto castle = castles.begin(); castle!=castles.end(); castle++) {
            if (abs(road->path[0].x - castle->pos.x) < 1 && abs(road->path[0].y - castle->pos.y) < 1) {
              castle->CancelAttack(road->path[1]);
            }
          }
          road = roads.erase(road);
        } else {
          road++;
        }
      }
    }
  }
}

void Control::Draw() {
  if (isHatching) {
    DrawLineEx(mousePressedPos, GetMousePosition(), 10.0f, GREEN);
  }
}