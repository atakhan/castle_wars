#include "Game.h"

Game::Game() {
  fight = Fight();
  castles.push_back(Castle({300, 300}, CW_BLUE, PLAYER));
  castles.push_back(Castle({700, 800}, CW_RED, AI));
  castles.push_back(Castle({550, 200}, CW_RED, AI));

  // roads.push_back(Road(std::vector<Vector2> { {300, 300}, {700, 800} }, CW_BLUE));
  // roads.push_back(Road(std::vector<Vector2> { {300, 300}, {550, 200} }, CW_BLUE));
  // roads.push_back(Road(std::vector<Vector2> { {700, 800}, {550, 200} }, CW_BLUE));
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
  fight.Update(castles, warriors, roads);

  Castle::UpdateAll(castles, warriors, roads);
  Warrior::UpdateAll(warriors);
}
