#ifndef CONTROL_H
#define CONTROL_H

#include "../../Configs.h"
#include "../Castle/Castle.h"
#include "../Road/Road.h"
#include <raylib-cpp.hpp>
#include <iostream>

namespace CW {

class Toolbar
{
 public:  
  Toolbar();
  void Update();
  void Draw();
};

}  // namespace CW

#endif  // CONTROL_H