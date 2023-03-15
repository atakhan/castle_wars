#include "Warrior.h"

Warrior::Warrior() {}

Warrior::Warrior(Vector2 _pos) {
  radius = 10;
  color = BLUE;
  vel.x = 0;
  vel.y = 0;
  speed = 1;
  angle = 0.0f;
  pos = _pos;
  path.push_back(Milestone({200,120}));
  path.push_back(Milestone({500,600}));
  path.push_back(Milestone({100,300}));
  path.push_back(Milestone({900,600}));
  currentMilestone = 0;
}

void Warrior::Move() {
  Vector2 targetPos = path.at(currentMilestone).pos;
  float dx = targetPos.x - pos.x;
  float dy = targetPos.y - pos.y;
  
  angle = atan2f(dy, dx);
  
  float dxx = speed * cosf(angle);
  float dyy = speed * sinf(angle);

  pos.x = pos.x + dxx;
  pos.y = pos.y + dyy;
  
  if (MilestoneReached(targetPos)) {
    if (currentMilestone + 1 < path.size()) {
      currentMilestone++;
    }
  }
}

bool Warrior::MilestoneReached(Vector2 targetPos) {
  if ( 
    fabs(pos.x - targetPos.x) < 1 &&
    fabs(pos.y - targetPos.y) < 1
  ) {
    std::cout << currentMilestone << " - " << targetPos.x << "," << targetPos.y << std::endl;
    return true;
  }
  return false;
}

void Warrior::Update() {
  Move();
}

void Warrior::Draw() {
  DrawCircle(
    pos.x,
    pos.y,
    radius,
    color
  );
}
