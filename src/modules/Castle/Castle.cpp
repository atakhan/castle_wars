#include "Castle.h"

Castle::Castle() {}

Castle::Castle(Vector2 _pos, Color _color, Fraction fraction_) {
  radius = 50.0f;
  color = _color;
  pos = _pos;
  fraction = fraction_;
  warriorsCount = 1;
  target = pos;
  isCurrent = false;
  status = DEFENSE;
  regen = 100;
  tick = 0;
  maxWarriors = 10;
}

void Castle::TakeDamage(Warrior &warrior) {
  if (fraction == warrior.fraction) {
    warriorsCount++;
  } else {
    warriorsCount--;
    if (warriorsCount < 0) {
      fraction = warrior.fraction;
      color = warrior.color;
      warriorsCount = 1;
    }
  }
}

void Castle::Attack(std::vector<Warrior> &warriors) {
  if (warriorsCount > 0 && status == ATTACK) {
    warriors.push_back(
      Warrior(pos, target, color, fraction)
    );
    warriorsCount--;
    status = DEFENSE;
  }
}

void Castle::AssignATarget(std::vector<Castle> &castles) {
  Vector2 mousePos = GetMousePosition();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    (CheckCollisionPointCircle(GetMousePosition(), pos, radius))
      ? isCurrent = true
      : isCurrent = false;
  }

  if (isCurrent && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
    // check if button not released on self
    if (CheckCollisionPointCircle(mousePos, pos, radius)) {
      isCurrent = false;
      return;
    }
    
    for (size_t i = 0; i < castles.size(); ++i) {
      if (CheckCollisionPointCircle(mousePos, castles[i].pos, castles[i].radius)) {
        target = castles[i].pos;
        status = ATTACK;
        break;
      }
    }
    isCurrent = false;
  }
}

void Castle::Regen() {
  if (warriorsCount < maxWarriors) {
    tick++;
    if (tick > regen) {
      warriorsCount++;
      tick = 0;
    }
  }
}

void Castle::Update(std::vector<Castle> &castles, std::vector<Warrior> &warriors) {
  if (warriorsCount > 0) {
    AssignATarget(castles);
    Attack(warriors);
  }
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
