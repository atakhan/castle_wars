#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 pos_, Color color_, Fraction fraction_, int level_) : level(level_) {
  fraction        = fraction_;
  color           = color_;
  pos             = pos_;
  radius          = CalculateRadiusByLevel();
  menuPos         = {pos_.x - radius, pos_.y + radius};
  menuRadius      = 30.0f;
  isCurrent       = false;
  showMenu        = false;
  warriorsCount   = 8;
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
      targets = std::vector<Vector2>();
      RemoveActiveRoads(roads);
    }
  }
}

void Castle::Attack(std::vector<Warrior> &warriors) {
  if (warriorsCount > 0 && targets.size() > 0) {
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
    for (size_t i = 0; i < castles.size(); ++i) {
      if (CheckCollisionPointCircle(mousePos, castles[i].pos, castles[i].radius)) {
        if (!RoadIsset(castles[i].pos, roads)) {
          targets.push_back(castles[i].pos);
          roads.push_back(Road(std::vector<Vector2> {pos, castles[i].pos}, color, fraction));
          break;
        }
      }
    }
    isCurrent = false;
}

void Castle::TryToCancelAttack(Vector2 target_) {
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

void Castle::UpgradeCastle() {
  if (
    CheckCollisionPointCircle(GetMousePosition(), menuPos, menuRadius) &&
    IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
  ) {
    if (warriorsCount >= level.getNextLevelCost() && level.current < 8) {
      warriorsCount = warriorsCount - level.getNextLevelCost();
      level.current++;
      radius = CalculateRadiusByLevel();
      menuPos = {pos.x - radius, pos.y + radius};
      GetLevelParameters();
    }
  }
}

void Castle::Update(std::vector<Warrior> &warriors) {
  UpgradeCastle();
  Attack(warriors);
  Regen();
}

void Castle::DrawCastle() {
  DrawCircle(pos.x, pos.y, radius, color);
  std::string warriorsCount_text = std::to_string(warriorsCount);
  raylib::DrawText(warriorsCount_text, pos.x, pos.y, 20, WHITE);
}

void Castle::DrawAttackPath() {
  if (isCurrent) {
    DrawLineEx(pos, GetMousePosition(), 4.0, color);
  }
}

void Castle::DrawMenu() {
  if (showMenu && level.current < 7) {
    DrawCircle(menuPos.x, menuPos.y, menuRadius, ColorAlpha(DARKGREEN, 0.85));
    DrawTextPro(GetFontDefault(), "UP", {menuPos.x - 8.0f, menuPos.y - 8.0f}, {0,0}, 0.0f, 16.0f, 3.0, WHITE);
  }
}

void Castle::Draw() {
  DrawCastle();
  DrawAttackPath();
  DrawMenu();
}
