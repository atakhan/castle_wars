#include "Warrior.h"

Warrior::Warrior() {}

Warrior::Warrior(Vector2 pos_, std::vector<Milestone> path_, Color color_) {
  radius = 10;
  color = color_;
  vel = {0, 0};
  speed = 1.0f;
  angle = 0.0f;
  pos = pos_;
  path = path_;
  currentMilestone = 0;
  status = IDLE;
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
}

bool Warrior::MilestoneReached(Vector2 targetPos) {
  if ( fabs(pos.x - targetPos.x) < 1 && fabs(pos.y - targetPos.y) < 1 ) {
    return true;
  }

  return false;
}

// void Warrior::Update(const GamePlayUi &ui) {
//   if (status == ATTACK) {
//     if (currentMilestone + 1 > path.size()) {
//       status = IDLE;
//       return;
//     }
//     Move();
//     if (MilestoneReached(path.at(currentMilestone).pos)) {
//       path.at(currentMilestone).reached = true;
//       currentMilestone++;
//     }
//   } 
  
//   if (status == RETREAT) {
//     if (currentMilestone < 0) {
//       status = IDLE;
//       return;
//     }
//     Move();
//     if (MilestoneReached(path.at(currentMilestone).pos)) {
//       path.at(currentMilestone).reached = true;
//       currentMilestone--;
//     }
//   }

  // if (ui.IsOrderButtonPressed()) {
  //   if (status == ATTACK) {
  //     SetStatus(RETREAT);
  //   } else if (status == RETREAT) {
  //     SetStatus(ATTACK);
  //   } else if (status == IDLE) {
  //     SetStatus(ATTACK);
  //   }
  // }
// }

void Warrior::SetStatus(Status newStatus) {
  status = newStatus;
}

void Warrior::Draw() {
  DrawCircle(
    pos.x,
    pos.y,
    radius,
    color
  );
}
