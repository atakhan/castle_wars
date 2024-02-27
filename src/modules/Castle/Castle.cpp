#include "Castle.h"

namespace CW {

Castle::Castle() {}

Castle::Castle(Vector2 pos, Color color, Fraction fraction, int level, RulesBook *rules) 
    : current_level_(level)
    , fraction_(fraction)
    , position_(pos)
    , regen_tick_(0)
    , attack_tick_(0)
    , is_current_(false)
    , warriors_count_(8)
    , color_(color)
    , rules_(rules)
    , menu_radius_(30.0f)
{
  UpdateParameters();
  menu_position_ = {
    position_.x - (menu_radius_ * (powf((current_level_ + 1), 0.5))),
    position_.y + (menu_radius_ * (powf((current_level_ + 1), 0.5)))
  };
}

void Castle::Update(std::vector<Warrior> &warriors) {
  Upgrade();
  Attack(warriors);
  Regen();
}

int Castle::GetCurrentLevel() {
  return current_level_;
}

int Castle::GetRadius() {
  return radius_;
}

Fraction Castle::GetFraction() {
  return fraction_;
}

Vector2 Castle::GetPosition() {
  return position_;
}

float Castle::CalculateRadiusByLevel() {
  return 30.0f * (powf((current_level_ + 1), 0.5));
}

int Castle::GetNextLevelCost() {
  return rules_->GetNextLevelCost(current_level_);
}

void Castle::SetMenuVisible(bool value) {
  show_menu_ = value;
}

bool Castle::GetMenuVisible() {
  return show_menu_;
}

void Castle::Upgrade() {
  if (!OnMenuPressed()) {
    return;
  }
  if ((current_level_ + 1) >= rules_->GetMaxLevel()) {
    return;
  }
  if (warriors_count_ < GetNextLevelCost()) {
    return;
  }
  warriors_count_ = warriors_count_ - GetNextLevelCost();
  current_level_++;
  UpdateParameters();
  SetMenuVisible(false);
}

bool Castle::CheckCollisionWithMouse(Vector2 mouse_pos) {
  if (CheckCollisionPointCircle(mouse_pos, GetPosition(), GetRadius())) {
    return true;
  }
  return false;
}

bool Castle::OnMenuPressed() {
  if (show_menu_ && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && 
      CheckCollisionPointCircle(GetMousePosition(), menu_position_, menu_radius_)) {
    return true;
  }
  return false;
}

void Castle::UpdateParameters() {
  radius_           = CalculateRadiusByLevel();
  max_warriors_     = rules_->GetCurrentMax(current_level_);
  regen_speed_      = rules_->GetCurrentRegen(current_level_);
  attack_frequency_ = rules_->GetCurrentAttackFrequency(current_level_);
}

void Castle::ChangeFraction(Warrior &warrior, std::vector<Road> &roads) {
  fraction_ = warrior.GetFraction();
  color_ = warrior.GetColor();
  warriors_count_ = 1;
  targets_.clear();
  RemoveActiveRoads(roads);
}

void Castle::RemoveActiveRoads(std::vector<Road> &roads) {
  for (auto road = roads.begin(); road != roads.end();) {
    if (abs(road->path[0].x - position_.x) < 1 && abs(road->path[0].y - position_.y) < 1) {
      road = roads.erase(road);
    } else {
      road++;
    }
  }
}

void Castle::TakeDamage(Warrior &warrior, std::vector<Road> &roads) {
  if (fraction_ == warrior.GetFraction()) {
    warriors_count_++;
  } else {
    warriors_count_--;
    if (warriors_count_ < 0) {
      ChangeFraction(warrior, roads);
    }
  }
}

void Castle::Attack(std::vector<Warrior> &warriors) {
  if (warriors_count_ > 0 && targets_.size() > 0) {
    attack_tick_++;
    if (attack_tick_ > attack_frequency_) {
      for (size_t i = 0; i < targets_.size(); ++i) {
        warriors.push_back(Warrior(position_, targets_[i], color_, fraction_));
        warriors_count_--;
      }
      attack_tick_ = 0;
    }
  }
}

void Castle::Regen() {
  if (fraction_ != NEUTRAL && warriors_count_ < max_warriors_) {
    regen_tick_++;
    if (regen_tick_ > regen_speed_) {
      warriors_count_++;
      regen_tick_ = 0;
    }
  }
}

bool Castle::RoadIsset(Vector2 end_pos, std::vector<Road> &roads) {
  bool isset = false;
  for (size_t i = 0; i < roads.size(); ++i) {
    if (
      (
        (abs(roads[i].path[0].x - position_.x) < 1) &&
        (abs(roads[i].path[0].y - position_.y) < 1) &&
        (abs(roads[i].path[1].x - end_pos.x) < 1) &&
        (abs(roads[i].path[1].y - end_pos.y) < 1) &&
        (roads[i].fraction == fraction_)
      ) || (
        (abs(roads[i].path[1].x - position_.x) < 1) &&
        (abs(roads[i].path[1].y - position_.y) < 1) &&
        (abs(roads[i].path[0].x - end_pos.x) < 1) &&
        (abs(roads[i].path[0].y - end_pos.y) < 1) &&
        (roads[i].fraction == fraction_)
      )
    ) {
      isset = true;
    }
  }
  return isset;
}

void Castle::TryToAssignATarget(Vector2 mousePos, std::vector<Castle> &castles, std::vector<Road> &roads) {
  for (size_t i = 0; i < castles.size(); ++i) {
    if (CheckCollisionPointCircle(mousePos, castles[i].position_, castles[i].radius_)) {
      if (!RoadIsset(castles[i].position_, roads)) {
        targets_.push_back(castles[i].position_);
        roads.push_back(Road(std::vector<Vector2> {position_, castles[i].position_}, color_, fraction_));
        break;
      }
    }
  }
  is_current_ = false;
}

void Castle::TryToCancelAttack(Vector2 target_castle) {
  for(auto target = targets_.begin(); target != targets_.end();) {
    if (
      (abs(target->x - target_castle.x) < 1) &&
      (abs(target->y - target_castle.y) < 1)
    ) {
      target = targets_.erase(target);
    } else {
      target++;
    }
  }
}

void Castle::Draw() {
  DrawCastle();
  DrawCastleLevel();
  DrawAttackPath();
  if (show_menu_ && current_level_ < rules_->GetMaxLevel()) {
    DrawMenu();
  }
}

void Castle::DrawCastleLevel() {
  DrawCircle(position_.x, position_.y - radius_, 20, BLACK);
  std::string current_level_str = std::to_string(current_level_);
  raylib::DrawText(current_level_str, position_.x - 6.0f, position_.y - radius_, 12.0f, WHITE);
}

void Castle::DrawCastle() {
  DrawCircle(position_.x, position_.y, radius_, color_);
  std::string warriors_count__text = std::to_string(warriors_count_);
  raylib::DrawText(warriors_count__text, position_.x, position_.y, 20, WHITE);
}

void Castle::DrawAttackPath() {
  if (is_current_) {
    DrawLineEx(position_, GetMousePosition(), 4.0, color_);
  }
}

void Castle::DrawMenu() {
  DrawCircle(menu_position_.x, menu_position_.y, menu_radius_, ColorAlpha(DARKGREEN, 0.85));
  std::string update_cost = std::to_string(GetNextLevelCost());
  raylib::DrawText(update_cost, menu_position_.x - 8.0f, menu_position_.y - 8.0f, 12.0f, WHITE);
}

}  // namespace CW
