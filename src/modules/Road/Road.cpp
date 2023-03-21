#include "Road.h"

Road::Road() {}

Road::Road(std::vector<Vector2> path_, Color color_) {
  path = path_;
  color = color_;
}

void Road::Draw() {
  for(unsigned int i = 0; i < path.size(); i++){
    if ( (i + 1) < path.size() ) {
      DrawLineEx(path[i], path[i+1], 10.0f, ColorAlpha(color, 0.2));
    }
  }
}