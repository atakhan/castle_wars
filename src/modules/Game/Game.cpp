#include "Game.h"

Game::Game() {
  warrior = Warrior({100, 100});
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Draw() {
  DrawStats();
  warrior.Draw();
  Warrior::DrawPath(warrior.path);
}

void Game::Update() {
  warrior.Update();
}