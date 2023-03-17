#ifndef ARMY_H
#define ARMY_H

#include <raylib-cpp.hpp>
#include <iostream>

class Army
{
public:
  unsigned int warriorsCount;

  Army();
  Army(Vector2 pos, Color color);

  void Update();
  void Draw();

  static void DrawAll(std::vector<Army> &armies) {
    for(auto army = armies.begin(); army!=armies.end(); army++)
      army->Draw();
  }

};

#endif  // ARMY_H