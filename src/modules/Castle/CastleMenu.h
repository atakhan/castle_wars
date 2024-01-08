#ifndef CASTLEMENU_H
#define CASTLEMENU_H

#include <iostream>
#include <math.h>

#include <raylib-cpp.hpp>

#include "../../Configs.h"


namespace CW {

class CastleMenu {
 public:
  CastleMenu(Vector2 castle_position, int level);

  void Update();
  bool UpgradeCastle();
  void Draw();
  bool Show();
 
 private:
  Vector2 position_;
  float radius_;
  bool show_;

};

}  // namespace CW

#endif  // CASTLEMENU_H