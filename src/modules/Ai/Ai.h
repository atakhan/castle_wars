#ifndef AI_H
#define AI_H

#include "../Warrior/Warrior.h"
#include <raylib-cpp.hpp>
#include <iostream>

namespace CW {

enum AiStatus {
  ATTACK,
  DEFENSE
};

class Ai
{
public:
  Ai();
};

}  // namespace CW

#endif  // AI_H