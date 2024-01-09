#include "Warrior.h"

namespace CW {

Warrior::Warrior() {}

Warrior::Warrior(Vector2 pos, Vector2 target_pos, Color color, Fraction fraction)
    : position_(pos)
    , home_castle_(pos)
    , target_(target_pos)
    , color_(color)
    , fraction_(fraction)
    , radius_(10)
    , velocity_{}
    , speed_(3.0f)
    , angle_(0.0f)
    , target_reached_(false)
{}

void Warrior::Move() {
  float dx = target_.x - position_.x;
  float dy = target_.y - position_.y;

  angle_ = atan2f(dy, dx);
  
  float dxx = speed_ * cosf(angle_);
  float dyy = speed_ * sinf(angle_);
  
  position_.x = position_.x + dxx;
  position_.y = position_.y + dyy;
}

bool Warrior::CastleReached() {
  if ( fabs(position_.x - target_.x) < 1 && fabs(position_.y - target_.y) < 1 ) {
    target_reached_ = true;
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
    position_.x,
    position_.y,
    radius_,
    color_
  );
}

int Warrior::GetRadius() {
  return radius_;
}

Vector2 Warrior::GetPosition() {
  return position_;
}

Vector2 Warrior::GetHomeCastlePosition() {
  return home_castle_;
}

Fraction Warrior::GetFraction() {
  return fraction_;
}

Color Warrior::GetColor() {
  return color_;
}

}  // namespace CW
