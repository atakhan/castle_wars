#include "Game.h"

Game::Game() {
  fight = Fight();
  control = Control();
  castles.push_back(Castle({300, 300}, CW_BLUE, PLAYER, 0));
  castles.push_back(Castle({700, 800}, CW_RED,  AI,     7));
  castles.push_back(Castle({550, 200}, CW_RED,  AI,     0));
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Update() {
  control.Update(castles, roads);
  fight.Update(castles, warriors, roads);
  Castle::UpdateAll(castles, warriors);
  Warrior::UpdateAll(warriors);
}

void Game::Draw() {
  DrawStats();
  Road::DrawAll(roads);
  Castle::DrawAll(castles);
  Warrior::DrawAll(warriors);
  control.Draw();
}
