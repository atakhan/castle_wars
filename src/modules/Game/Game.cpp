#include "Game.h"

Game::Game() {
  fight = Fight();
  castles.push_back(Castle({200, 200}, CW_BLUE, PLAYER));
  castles.push_back(Castle({800, 800}, CW_RED, AI));

  road = Road(
    std::vector<Milestone> {
      Milestone({200, 200}),
      Milestone({800, 800})
    }
  );
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Draw() {
  DrawStats();
  road.Draw();
  Castle::DrawAll(castles);
  Warrior::DrawAll(warriors);
}

void Game::Update() {
  Warrior::UpdateAll(warriors);
  Castle::UpdateAll(castles, warriors);
  fight.Update(castles, warriors);
}
