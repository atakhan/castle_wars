#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 _pos, Color _color) {
  radius = 50.0f;
  color = _color;
  pos = _pos;
  
  warriorsCount = 1;
  
  target = pos;
  isCurrent = false;
  status = DEFENSE;
}

void Castle::Attack(std::vector<Warrior> &warriors) {
  if (warriorsCount > 0 && status == ATTACK) {
    warriors.push_back(Warrior(pos, target, color));
    warriorsCount--;
  }
}

void Castle::AssignATarget(std::vector<Castle> &castles) {
  Vector2 mousePos = GetMousePosition();
  
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    (CheckCollisionPointCircle(GetMousePosition(), pos, radius))
      ? isCurrent = true
      : isCurrent = false;
  }

  if (isCurrent && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    for (size_t i = 0; i < castles.size(); ++i) {
      if (CheckCollisionPointCircle(mousePos, castles[i].pos, castles[i].radius)) {
        target = castles[i].pos;
        status = ATTACK;
        break;
      }
    }
    isCurrent = false;
  }
}

void Castle::Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors) {
  AssignATarget(castles);
  Attack(warriors);
}

void Castle::Draw() {
  DrawCircle(pos.x, pos.y, radius, color);
  if (isCurrent) {
    DrawLineEx(pos, GetMousePosition(), 4.0, color);
  }
}
