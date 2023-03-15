#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 _pos) {
  radius = 30;
  color = BLUE;
  vel.x = 0;
  vel.y = 0;
  speed = 1;
  angle = 0.0f;
  pos = _pos;
}

void Castle::Update() {
  
}

void Castle::Draw() {
  DrawCircle(
    pos.x,
    pos.y,
    radius,
    color
  );
}
