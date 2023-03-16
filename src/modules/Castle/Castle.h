#ifndef CASTLE_H
#define CASTLE_H

#include <raylib-cpp.hpp>
#include <iostream>

class Castle
{
public:
  Vector2 pos;
  Color color;
  float width;
  float height;

  Castle();
  Castle(Vector2 pos, Color color);

  void Update();
  void Draw();

};

#endif  // CASTLE_H