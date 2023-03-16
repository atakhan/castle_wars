#include "Warrior.h"

Warrior::Warrior() {}

Warrior::Warrior(Vector2 _pos, std::vector<Milestone> _path) {
  radius = 10;
  color = CW_BLUE;
  vel.x = 0;
  vel.y = 0;
  speed = 1.0f;
  angle = 0.0f;
  pos = _pos;
  path = _path;
  currentMilestone = 0;
  status = IDLE;
}

void Warrior::Move() {
  if (currentMilestone + 1 > path.size()) {
    status = IDLE;
    return;
  }
  Vector2 targetPos = path.at(currentMilestone).pos;
  float dx = targetPos.x - pos.x;
  float dy = targetPos.y - pos.y;

  angle = atan2f(dy, dx);
  
  float dxx = speed * cosf(angle);
  float dyy = speed * sinf(angle);

  pos.x = pos.x + dxx;
  pos.y = pos.y + dyy;
  
  if (MilestoneReached(targetPos)) {
    path.at(currentMilestone).reached = true;
    currentMilestone++;
  }
}

bool Warrior::MilestoneReached(Vector2 targetPos) {
  if ( fabs(pos.x - targetPos.x) < 1 && fabs(pos.y - targetPos.y) < 1 ) {
    return true;
  }

  return false;
}

void Warrior::Update() {
  if (status == ATTACK) {
    Move();
  }
}

void Warrior::SetAttackStatus() {
  status = ATTACK;
}

void Warrior::Draw() {
  DrawCircle(
    pos.x,
    pos.y,
    radius,
    color
  );
}
