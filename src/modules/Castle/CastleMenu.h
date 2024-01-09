#ifndef CASTLEMENU_H
#define CASTLEMENU_H

#include <iostream>
#include <math.h>

#include <raylib-cpp.hpp>

#include "../../Configs.h"
#include "CastleLevel.h"


namespace CW {

class CastleMenu {
 public:
  CastleMenu(Vector2 castle_position, CastleLevel *level);

  void Update();
  bool UpgradeCastle();
  void Draw();
  bool Show();
  void SetVisible(bool value);
 
 private:
  CastleLevel *level_;
  Vector2 position_;
  float radius_;
  bool show_;

};

}  // namespace CW

#endif  // CASTLEMENU_H