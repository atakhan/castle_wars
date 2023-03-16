#include "Milestone.h"

Milestone::Milestone(Vector2 _pos) {
  pos = _pos;
  reached = false;
}

void Milestone::Draw() {
  if (reached == false) {
    DrawCircle(
      pos.x, pos.y,
      5, CW_RED
    );
  }
}