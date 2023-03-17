#include "Army.h"

Army::Army() {}

Army::Army(Vector2 _pos, Color _color) {
  radius = 50.0f;
  color = _color;
  pos = _pos;
}

void Army::Update() {
  
}

void Army::Draw() {
  DrawCircle(pos.x, pos.y, radius, color);
}
