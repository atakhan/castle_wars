#include "Road.h"

Road::Road() {}

Road::Road(std::vector<Milestone> path_) {
  path = path_;
}

void Road::Draw() {
  for(unsigned int i = 0; i < path.size(); i++){
    if ( (i + 1) < path.size() ) {
      DrawLineEx(path[i].pos, path[i+1].pos, 10.0f, CW_ROAD);
    }
  }
}