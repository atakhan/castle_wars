#include "Game.h"

Game::Game() {
  fight = Fight();
  castles.push_back(Castle({300, 300}, CW_BLUE, PLAYER));
  castles.push_back(Castle({700, 800}, CW_RED, AI));
  castles.push_back(Castle({550, 200}, CW_RED, AI));

  roads.push_back(Road(std::vector<Milestone> { Milestone({300, 300}), Milestone({700, 800}) }));
  roads.push_back(Road(std::vector<Milestone> { Milestone({300, 300}), Milestone({550, 200}) }));
  roads.push_back(Road(std::vector<Milestone> { Milestone({700, 800}), Milestone({550, 200}) }));
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Draw() {
  DrawStats();

  Road::DrawAll(roads);
  Castle::DrawAll(castles);
  Warrior::DrawAll(warriors);
}

void Game::Update() {
  fight.Update(castles, warriors);

  Castle::UpdateAll(castles, warriors);
  Warrior::UpdateAll(warriors);
}
