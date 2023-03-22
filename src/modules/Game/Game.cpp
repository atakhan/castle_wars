#include "Game.h"

Game::Game() {
  fight = Fight();
  control = Control();
  castles.push_back(Castle({300, 300}, CW_BLUE, PLAYER));
  castles.push_back(Castle({700, 800}, CW_RED, AI));
  castles.push_back(Castle({550, 200}, CW_RED, AI));
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Draw() {
  DrawStats();
  Road::DrawAll(roads);
  Castle::DrawAll(castles);
  Warrior::DrawAll(warriors);
  control.Draw();
}

void Game::Update() {
  control.Update(castles, roads);
  fight.Update(castles, warriors, roads);
  Castle::UpdateAll(castles, warriors);
  Warrior::UpdateAll(warriors);
}
