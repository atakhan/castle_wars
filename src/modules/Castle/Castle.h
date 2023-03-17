#ifndef CASTLE_H
#define CASTLE_H

#include <raylib-cpp.hpp>
#include <iostream>

class Castle
{
public:
  float radius;
  Vector2 pos;
  Color color;

  Castle();
  Castle(Vector2 pos, Color color);

  void Update();
  void Draw();

  static void DrawAll(std::vector<Castle> &castles) {
    for(auto castle = castles.begin(); castle!=castles.end(); castle++)
      castle->Draw();
  }

};

#endif  // CASTLE_H