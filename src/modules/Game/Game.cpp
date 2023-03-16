#include "Game.h"

Game::Game() {
  blueCastle = Castle({100, 100}, CW_BLUE);
  redCastle = Castle({900, 900}, CW_RED);
  GeneratePath();
  warrior = Warrior({100, 100}, path);
  ui = GamePlayUi();
  road = Road(path);
}

void Game::GeneratePath() {
  path.push_back(Milestone({100,100}));
  path.push_back(Milestone({150,210}));
  path.push_back(Milestone({320,260}));
  path.push_back(Milestone({430,430}));
  path.push_back(Milestone({540,540}));
  path.push_back(Milestone({630,630}));
  path.push_back(Milestone({720,790}));
  path.push_back(Milestone({810,810}));
  path.push_back(Milestone({900,900}));
}

void Game::DrawStats() {
  DrawFPS(10,10);
}

void Game::Draw() {
  DrawStats();
  road.Draw();
  warrior.Draw();
  blueCastle.Draw();
  redCastle.Draw();
  ui.Draw();
  Warrior::DrawPath(warrior.path);
}

void Game::Update() {
  warrior.Update();
  if (ui.IsAttackButtonPressed()) {
    warrior.SetAttackStatus();
  }
}
