#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 pos_, Color color_, Fraction fraction_, int level_) : level(level_) {
  fraction        = fraction_;
  color           = color_;
  pos             = pos_;
  radius          = CalculateRadiusByLevel();
  isCurrent       = false;
  status          = DEFENSE;
  warriorsCount   = 1;
  regenTick       = 0;
  attackTick      = 0;
  GetLevelParameters();
}

float Castle::CalculateRadiusByLevel() {
  return 30.0f * (powf((level.current + 1), 0.5));
}

void Castle::GetLevelParameters() {
  maxWarriors     = level.getCurrentMax();
  regenSpeed      = level.getCurrentRegen();
  attackFrequency = level.getCurrentAttackFrequency();
}

void Castle::RemoveActiveRoads(std::vector<Road> &roads) {
  for (auto road = roads.begin(); road != roads.end();) {
    if (abs(road->path[0].x - pos.x) < 1 && abs(road->path[0].y - pos.y) < 1) {
      road = roads.erase(road);
    } else {
      road++;
    }
  }
}

void Castle::TakeDamage(Warrior &warrior, std::vector<Road> &roads) {
  if (fraction == warrior.fraction) {
    warriorsCount++;
  } else {
    warriorsCount--;
    if (warriorsCount < 0) {
      fraction = warrior.fraction;
      color = warrior.color;
      warriorsCount = 1;
      status = DEFENSE;
      RemoveActiveRoads(roads);
    }
  }
}

void Castle::Attack(std::vector<Warrior> &warriors) {
  if (warriorsCount > 0 && status == ATTACK) {
    attackTick++;
    if (attackTick > attackFrequency) {
      for (size_t i = 0; i < targets.size(); ++i) {
        warriors.push_back(Warrior(pos, targets[i], color, fraction));
        warriorsCount--;
      }
      attackTick = 0;
    }
  }
}

void Castle::Regen() {
  if (warriorsCount < maxWarriors) {
    regenTick++;
    if (regenTick > regenSpeed) {
      warriorsCount++;
      regenTick = 0;
    }
  }
}

bool Castle::RoadIsset(Vector2 endPos, std::vector<Road> &roads)
{
  bool isset = false;
  for (size_t i = 0; i < roads.size(); ++i) {
    if (
      (
        (abs(roads[i].path[0].x - pos.x) < 1) &&
        (abs(roads[i].path[0].y - pos.y) < 1) &&
        (abs(roads[i].path[1].x - endPos.x) < 1) &&
        (abs(roads[i].path[1].y - endPos.y) < 1) &&
        (roads[i].fraction == fraction)
      ) || (
        (abs(roads[i].path[1].x - pos.x) < 1) &&
        (abs(roads[i].path[1].y - pos.y) < 1) &&
        (abs(roads[i].path[0].x - endPos.x) < 1) &&
        (abs(roads[i].path[0].y - endPos.y) < 1) &&
        (roads[i].fraction == fraction)
      )
    ) {
      isset = true;
    }
  }
  return isset;
}

void Castle::TryToAssignATarget(Vector2 mousePos, std::vector<Castle> &castles, std::vector<Road> &roads) {
    // don't attack own!
    if (CheckCollisionPointCircle(mousePos, pos, radius)) {
      isCurrent = false;
      return;
    }

    for (size_t i = 0; i < castles.size(); ++i) {
      if (CheckCollisionPointCircle(mousePos, castles[i].pos, castles[i].radius)) {
        if (!RoadIsset(castles[i].pos, roads)) {
          targets.push_back(castles[i].pos);
          status = ATTACK;
          roads.push_back(Road(std::vector<Vector2> {pos, castles[i].pos}, color, fraction));
          break;
        }
      }
    }
    isCurrent = false;
}

void Castle::CancelAttack(Vector2 target_) {
  status = DEFENSE;
  for(auto target = targets.begin(); target != targets.end();) {
    if (
      (abs(target->x - target_.x) < 1) &&
      (abs(target->y - target_.y) < 1)
    ) {
      target = targets.erase(target);
    } else {
      target++;
    }
  }
}

void Castle::Update(std::vector<Warrior> &warriors) {
  Attack(warriors);
  Regen();
}

void Castle::Draw() {
  DrawCircle(pos.x, pos.y, radius, color);
  if (isCurrent) {
    DrawLineEx(pos, GetMousePosition(), 4.0, color);
  }
  std::string warriorsCount_text = std::to_string(warriorsCount);
  raylib::DrawText(warriorsCount_text, pos.x, pos.y, 20, WHITE);
}
