#ifndef WARRIOR_H
#define WARRIOR_H

#include "../../Colors.h"
#include "../Milestone/Milestone.h"
#include "../GamePlayUi/GamePlayUi.h"
#include <raylib-cpp.hpp>
#include <iostream>
#include <limits>

enum Status {
  IDLE,
  ATTACK,
  RETREAT
};

class Warrior
{
public:
  int radius;
  Vector2 pos;
  Vector2 vel;
  Color color;
  float speed = 1;
  float angle;
  std::vector<Milestone> path;
  int currentMilestone;

  Status status;

  Warrior();
  Warrior(Vector2 pos, std::vector<Milestone> path, Color color);

  void SetStatus(Status newStatus);
  void Move();
  bool MilestoneReached(Vector2 targetPos);
  // void Update(const GamePlayUi &ui);
  void Draw();

  static void DrawPath(std::vector<Milestone> &path) {
    for(auto milestone = path.begin(); milestone != path.end(); milestone++)
      milestone->Draw();
  }

};

#endif  // WARRIOR_H