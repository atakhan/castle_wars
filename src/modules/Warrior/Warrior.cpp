#include "Warrior.h"

Warrior::Warrior() {}

Warrior::Warrior(Vector2 pos_, Vector2 targetPos, Color color_, Fraction fraction_) {
  radius = 10;
  color = color_;
  vel = {0, 0};
  speed = 1.0f;
  angle = 0.0f;
  pos = pos_;
  target = targetPos;
  fraction = fraction_;
  targetReached = false;
}

void Warrior::Move() {
  float dx = target.x - pos.x;
  float dy = target.y - pos.y;

  angle = atan2f(dy, dx);
  
  float dxx = speed * cosf(angle);
  float dyy = speed * sinf(angle);
  
  pos.x = pos.x + dxx;
  pos.y = pos.y + dyy;
}

bool Warrior::CastleReached() {
  if ( fabs(pos.x - target.x) < 1 && fabs(pos.y - target.y) < 1 ) {
    targetReached = true;
    return true;
  }
  return false;
}

void Warrior::Update() {
  if (!CastleReached()) {
    Move();
  }
}

void Warrior::Draw() {
  DrawCircle(
    pos.x,
    pos.y,
    radius,
    color
  );
}
