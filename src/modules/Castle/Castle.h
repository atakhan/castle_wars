#ifndef CASTLE_H
#define CASTLE_H

#include <raylib-cpp.hpp>
#include <iostream>

class Castle
{
public:
  int radius;
  Vector2 pos;
  Vector2 vel;
  Color color;
  float speed = 1;
  float angle;

  Castle();
  Castle(Vector2 pos);

  void Move();
  void Update();
  void Draw();

};

#endif  // CASTLE_H