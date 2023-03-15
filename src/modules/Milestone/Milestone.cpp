#include "Milestone.h"

Milestone::Milestone(Vector2 _pos) {
  pos = _pos;
  reached = false;
}

void Milestone::Draw() {
  DrawCircle(
    pos.x,
    pos.y,
    5,
    RED
  );
}