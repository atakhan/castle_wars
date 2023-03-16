#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 _pos, Color _color) {
  color = _color;
  width = 80.0f;
  height = 80.0f;
  pos.x = _pos.x - (width / 2);
  pos.y = _pos.y - (height / 2);
}

void Castle::Update() {
  
}

void Castle::Draw() {
  Rectangle castle = {pos.x, pos.y, width, height};
  DrawRectangleLinesEx(
    castle, 
    5.0f,
    color
  );
}
