#include "GameObjects.h"

namespace CW {

GameObjects::GameObjects()
    : roads_()
    , castles_()
    , warriors_() {}

void GameObjects::AddCastle(const Castle &castle) {
  castles_.push_back(castle);
}

std::vector<Castle> &GameObjects::GetCastles() {
  return castles_;
}

std::vector<Road> &GameObjects::GetRoads() {
  return roads_;
}

std::vector<Warrior> &GameObjects::GetWarriors() {
  return warriors_;
}

void GameObjects::Update() {
  for (auto castle = castles_.begin(); castle != castles_.end(); castle++) {
    castle->Update(warriors_);
  }
  for(auto warrior = warriors_.begin(); warrior != warriors_.end(); warrior++) {
    warrior->Update();
  }
}

void GameObjects::Draw() {
  for (auto castle = castles_.begin(); castle != castles_.end(); castle++) {
    castle->Draw();
  }
  for(auto road = roads_.begin(); road != roads_.end(); road++) {
    road->Draw();
  }
  for(auto warrior = warriors_.begin(); warrior != warriors_.end(); warrior++) {
    warrior->Draw();
  }
}

}  // namespace CW
