#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 _pos, Color _color) {
  radius = 50.0f;
  color = _color;
  pos = _pos;
}

void Castle::Update() {
  
}

void Castle::Draw() {
  DrawCircle(pos.x, pos.y, radius, color);
}
